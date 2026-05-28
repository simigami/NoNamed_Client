#pragma once

#include "CoreMinimal.h"
#include "NoNamed/Core/NN_ObjectBase.h"
#include "NoNamed/Interface/NN_InventoryItemInterface.h"
#include "NN_ItemObjectBase.generated.h"

class UNN_ItemDataAsset;

/**
 * Base UObject logic instance for obtainable items.
 */
UCLASS(Blueprintable)
class NONAMED_API UNN_ItemObjectBase : public UNN_ObjectBase, public INN_InventoryItemInterface
{
	GENERATED_BODY()

public:
	const UNN_ItemDataAsset* GetItemData() const;

	//~ Begin INN_InventoryItemInterface interface
	virtual bool CanPickup(const UNN_InventoryComponent* Inventory) const override;
	virtual void OnPickedUp(UNN_InventoryComponent* Inventory) override;
	virtual void OnDropped(UNN_InventoryComponent* Inventory) override;
	//~ End INN_InventoryItemInterface interface
};
