#include "NN_InventoryComponent.h"
#include "NN_ItemObjectBase.h"
#include "NN_ItemDataAsset.h"
#include "NoNamed/Interface/NN_InventoryItemInterface.h"
#include "NoNamed/Character/NN_EquipmentComponent.h"
#include "NoNamed/Weapon/NN_WeaponObjectBase.h"

UNN_InventoryComponent::UNN_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UNN_InventoryComponent::CanPickup(const UNN_ItemObjectBase* Item) const
{
	// TODO: IOCP, Move authoritative pickup validation to the server.
	if (!Item)
	{
		return false;
	}

	const UNN_ItemDataAsset* ItemData = Item->GetItemData();
	const float ItemWeight = ItemData ? ItemData->Weight : 0.0f;

	if (CurrentWeight + ItemWeight > MaxWeight)
	{
		return false;
	}

	// If it's a weapon, check equipment component slot availability
	if (const UNN_WeaponObjectBase* Weapon = Cast<UNN_WeaponObjectBase>(Item))
	{
		if (const UNN_EquipmentComponent* EquipComp = GetOwner()->FindComponentByClass<UNN_EquipmentComponent>())
		{
			const ENN_WeaponClass WClass = Weapon->GetWeaponClass();
			if (WClass == ENN_WeaponClass::Primary || WClass == ENN_WeaponClass::Secondary || WClass == ENN_WeaponClass::Melee)
			{
				if (!EquipComp->CanEquipLoadout(Weapon))
				{
					return false;
				}
			}
		}
	}

	// Check custom item hook from the interface
	const INN_InventoryItemInterface* ItemInterface = Cast<INN_InventoryItemInterface>(Item);
	if (ItemInterface && !ItemInterface->CanPickup(this))
	{
		return false;
	}

	return true;
}

bool UNN_InventoryComponent::PickupItem(UNN_ItemObjectBase* Item)
{
	// TODO: IOCP, Server should authorize inventory ownership changes.
	if (!CanPickup(Item))
	{
		return false;
	}

	bool bHandledByEquip = false;

	// If it's a weapon, attempt to equip it to Loadout first if it belongs to slot-managed class
	if (UNN_WeaponObjectBase* Weapon = Cast<UNN_WeaponObjectBase>(Item))
	{
		const ENN_WeaponClass WClass = Weapon->GetWeaponClass();
		if (WClass == ENN_WeaponClass::Primary || WClass == ENN_WeaponClass::Secondary || WClass == ENN_WeaponClass::Melee)
		{
			if (UNN_EquipmentComponent* EquipComp = GetOwner()->FindComponentByClass<UNN_EquipmentComponent>())
			{
				bHandledByEquip = EquipComp->EquipLoadout(Weapon);
			}
		}
	}

	if (!bHandledByEquip)
	{
		Items.Add(Item);
	}

	// Destroy the world actor when picked up, unless it is held in hand as the active weapon
	if (Item->SpawnedActor)
	{
		bool bKeepActor = false;
		if (UNN_WeaponObjectBase* Weapon = Cast<UNN_WeaponObjectBase>(Item))
		{
			if (UNN_EquipmentComponent* EquipComp = GetOwner()->FindComponentByClass<UNN_EquipmentComponent>())
			{
				if (EquipComp->GetEquippedWeapon() == Weapon)
				{
					bKeepActor = true;
				}
			}
		}

		if (!bKeepActor)
		{
			Item->SpawnedActor->Destroy();
			Item->SpawnedActor = nullptr;
		}
	}

	OnItemAdded(Item);

	// Execute item object-level hook
	INN_InventoryItemInterface* ItemInterface = Cast<INN_InventoryItemInterface>(Item);
	if (ItemInterface)
	{
		ItemInterface->OnPickedUp(this);
	}

	return true;
}

bool UNN_InventoryComponent::DropItem(UNN_ItemObjectBase* Item)
{
	// TODO: IOCP, Server should authorize item drops and world item spawning.
	if (!Item)
	{
		return false;
	}

	bool bRemoved = false;

	UNN_WeaponObjectBase* Weapon = Cast<UNN_WeaponObjectBase>(Item);
	if (Weapon)
	{
		UNN_EquipmentComponent* EquipComp = GetOwner()->FindComponentByClass<UNN_EquipmentComponent>();
		if (EquipComp && EquipComp->HasWeapon(Weapon))
		{
			bRemoved = EquipComp->UnequipLoadout(Weapon);
		}
	}

	// If not managed by EquipmentComponent, remove from inventory Items array
	if (!bRemoved)
	{
		if (Items.Contains(Item))
		{
			Items.Remove(Item);
			bRemoved = true;
		}
	}

	if (!bRemoved)
	{
		return false;
	}

	// Drop을 할 때 액터는 다시 만들어 져야 함
	if (!Item->SpawnedActor)
	{
		Item->SpawnIntoWorld(GetWorld(), GetOwner()->GetActorTransform());
	}
	else
	{
		Item->SpawnedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Item->SpawnedActor->SetActorTransform(GetOwner()->GetActorTransform());
	}

	OnItemRemoved(Item);

	// Execute item object-level hook
	INN_InventoryItemInterface* ItemInterface = Cast<INN_InventoryItemInterface>(Item);
	if (ItemInterface)
	{
		ItemInterface->OnDropped(this);
	}

	return true;
}

void UNN_InventoryComponent::OnItemAdded(UNN_ItemObjectBase* Item)
{
	RecalcWeight();
	UE_LOG(LogTemp, Log, TEXT("UNN_InventoryComponent::OnItemAdded - Weight: %f / %f"), CurrentWeight, MaxWeight);
}

void UNN_InventoryComponent::OnItemRemoved(UNN_ItemObjectBase* Item)
{
	RecalcWeight();
	UE_LOG(LogTemp, Log, TEXT("UNN_InventoryComponent::OnItemRemoved - Weight: %f / %f"), CurrentWeight, MaxWeight);
}

void UNN_InventoryComponent::RecalcWeight()
{
	float NewWeight = 0.0f;
	for (const TObjectPtr<UNN_ItemObjectBase>& Item : Items)
	{
		if (Item)
		{
			const UNN_ItemDataAsset* ItemData = Item->GetItemData();
			NewWeight += ItemData ? ItemData->Weight : 0.0f;
		}
	}

	if (const UNN_EquipmentComponent* EquipComp = GetOwner()->FindComponentByClass<UNN_EquipmentComponent>())
	{
		NewWeight += EquipComp->GetEquipmentWeight();
	}

	CurrentWeight = NewWeight;
}
