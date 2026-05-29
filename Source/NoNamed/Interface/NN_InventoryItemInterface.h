#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NN_InventoryItemInterface.generated.h"

class UNN_InventoryComponent;

UINTERFACE(MinimalAPI, BlueprintType)
class UNN_InventoryItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface implemented by item objects to hook into inventory pickup and drop lifecycles.
 */
class NONAMED_API INN_InventoryItemInterface
{
	GENERATED_BODY()

public:
	/** Checks if this item can be picked up by the given inventory */
	virtual bool CanPickup(const UNN_InventoryComponent* Inventory) const;

	/** Callback hook executed when the item is successfully picked up */
	virtual void OnPickedUp(UNN_InventoryComponent* Inventory);

	/** Callback hook executed when the item is dropped from the inventory */
	virtual void OnDropped(UNN_InventoryComponent* Inventory);
};
