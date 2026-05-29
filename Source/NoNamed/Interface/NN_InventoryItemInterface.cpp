#include "NN_InventoryItemInterface.h"

bool INN_InventoryItemInterface::CanPickup(const UNN_InventoryComponent* Inventory) const
{
	return true;
}

void INN_InventoryItemInterface::OnPickedUp(UNN_InventoryComponent* Inventory)
{
}

void INN_InventoryItemInterface::OnDropped(UNN_InventoryComponent* Inventory)
{
}
