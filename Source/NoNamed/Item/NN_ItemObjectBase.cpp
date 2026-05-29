#include "NN_ItemObjectBase.h"

#include "NN_ItemDataAsset.h"

const UNN_ItemDataAsset* UNN_ItemObjectBase::GetItemData() const
{
	return Cast<UNN_ItemDataAsset>(DataAsset);
}

bool UNN_ItemObjectBase::CanPickup(const UNN_InventoryComponent* Inventory) const
{
	return true;
}

void UNN_ItemObjectBase::OnPickedUp(UNN_InventoryComponent* Inventory)
{
	UE_LOG(LogTemp, Log, TEXT("UNN_ItemObjectBase::OnPickedUp - Item picked up."));
}

void UNN_ItemObjectBase::OnDropped(UNN_InventoryComponent* Inventory)
{
	UE_LOG(LogTemp, Log, TEXT("UNN_ItemObjectBase::OnDropped - Item dropped."));
}
