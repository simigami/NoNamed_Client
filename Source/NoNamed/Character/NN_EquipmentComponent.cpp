#include "NN_EquipmentComponent.h"
#include "NoNamed/Weapon/NN_WeaponObjectBase.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

UNN_EquipmentComponent::UNN_EquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UNN_EquipmentComponent::CanEquipLoadout(const UNN_WeaponObjectBase* Weapon) const
{
	// TODO: IOCP, Move authoritative loadout validation to the server.
	if (!Weapon)
	{
		return false;
	}

	switch (const ENN_WeaponClass WeaponClass = Weapon->GetWeaponClass())
	{
		case ENN_WeaponClass::Primary:
			return Primaries.Num() < 2;
		case ENN_WeaponClass::Secondary:
			return Secondary == nullptr;
		case ENN_WeaponClass::Melee:
			return Melee == nullptr;
		default:
			break;
	}

	return false;
}

bool UNN_EquipmentComponent::EquipLoadout(UNN_WeaponObjectBase* Weapon)
{
	// TODO: IOCP, Server should authorize loadout slot changes.
	if (!CanEquipLoadout(Weapon))
	{
		return false;
	}

	bool bAdded = false;
	switch (const ENN_WeaponClass WeaponClass = Weapon->GetWeaponClass())
	{
		case ENN_WeaponClass::Primary:
			Primaries.Add(Weapon);
			bAdded = true;
			break;
		case ENN_WeaponClass::Secondary:
			Secondary = Weapon;
			bAdded = true;
			break;
		case ENN_WeaponClass::Melee:
			Melee = Weapon;
			bAdded = true;
			break;
		default:
			break;
	}

	return bAdded;
}

bool UNN_EquipmentComponent::UnequipLoadout(UNN_WeaponObjectBase* Weapon)
{
	// TODO: IOCP, Server should authorize loadout removal.
	if (!Weapon)
	{
		return false;
	}

	bool bRemoved = false;
	if (Primaries.Contains(Weapon))
	{
		Primaries.Remove(Weapon);
		bRemoved = true;
	}
	else if (Secondary == Weapon)
	{
		Secondary = nullptr;
		bRemoved = true;
	}
	else if (Melee == Weapon)
	{
		Melee = nullptr;
		bRemoved = true;
	}

	if (bRemoved)
	{
		if (ActiveWeapon == Weapon)
		{
			UnequipHand();
		}
	}

	return bRemoved;
}

bool UNN_EquipmentComponent::EquipHand(UNN_WeaponObjectBase* Weapon)
{
	// Must be managed in loadout before holding in hand, or can be null to clear
	if (Weapon && !HasWeapon(Weapon))
	{
		return false;
	}

	if (ActiveWeapon == Weapon)
	{
		return true;
	}

	// Put away current active weapon
	UnequipHand();

	ActiveWeapon = Weapon;

	if (ActiveWeapon)
	{
		AttachWeaponActor(ActiveWeapon);
	}

	return true;
}

void UNN_EquipmentComponent::UnequipHand()
{
	if (ActiveWeapon)
	{
		DetachWeaponActor(ActiveWeapon);
		if (ActiveWeapon->SpawnedActor)
		{
			ActiveWeapon->SpawnedActor->Destroy();
			ActiveWeapon->SpawnedActor = nullptr;
		}
		ActiveWeapon = nullptr;
	}
}

bool UNN_EquipmentComponent::HasWeapon(const UNN_WeaponObjectBase* Weapon) const
{
	if (!Weapon)
	{
		return false;
	}

	return Primaries.Contains(Weapon) || Secondary == Weapon || Melee == Weapon;
}

UNN_WeaponObjectBase* UNN_EquipmentComponent::GetEquippedWeapons() const
{
	return ActiveWeapon;
}

float UNN_EquipmentComponent::GetEquipmentWeight() const
{
	float WeightSum = 0.0f;

	for (const TObjectPtr<UNN_WeaponObjectBase>& Prim : Primaries)
	{
		if (Prim && Prim->GetItemData())
		{
			WeightSum += Prim->GetItemData()->Weight;
		}
	}

	if (Secondary && Secondary->GetItemData())
	{
		WeightSum += Secondary->GetItemData()->Weight;
	}

	if (Melee && Melee->GetItemData())
	{
		WeightSum += Melee->GetItemData()->Weight;
	}

	return WeightSum;
}

void UNN_EquipmentComponent::AttachWeaponActor(UNN_WeaponObjectBase* Weapon)
{
	if (!Weapon)
	{
		return;
	}

	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		// Spawn the actor if it doesn't exist yet
		if (!Weapon->SpawnedActor)
		{
			Weapon->SpawnIntoWorld(GetWorld(), Character->GetActorTransform());
		}

		if (AActor* WeaponActor = Weapon->SpawnedActor)
		{
			WeaponActor->SetOwner(Character);
			WeaponActor->SetInstigator(Character);

			const UNN_WeaponDataAsset* WeaponData = Cast<UNN_WeaponDataAsset>(Weapon->DataAsset);
			const FName SocketName = WeaponData ? WeaponData->AttachSocketName : NAME_None;
			USkeletalMeshComponent* CharacterMesh = Character->GetMesh();
			if (SocketName != NAME_None && CharacterMesh && CharacterMesh->DoesSocketExist(SocketName))
			{
				WeaponActor->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
				return;
			}

			const FVector FallbackLocation = Character->GetActorLocation()
				+ Character->GetActorForwardVector() * 100.0f
				+ FVector(0.0f, 0.0f, 50.0f);
			WeaponActor->SetActorLocation(FallbackLocation);
			WeaponActor->SetActorRotation(Character->GetActorRotation());

			UE_LOG(LogTemp, Warning, TEXT("UNN_EquipmentComponent::AttachWeaponActor - Weapon '%s' has no valid AttachSocketName. Using fallback location."),
				*GetNameSafe(Weapon));
		}
	}
}

void UNN_EquipmentComponent::DetachWeaponActor(UNN_WeaponObjectBase* Weapon)
{
	if (!Weapon)
	{
		return;
	}

	if (AActor* WActor = Weapon->SpawnedActor)
	{
		WActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}
