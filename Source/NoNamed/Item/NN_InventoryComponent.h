#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NN_InventoryComponent.generated.h"

class UNN_ItemObjectBase;

/**
 * Component to manage a character's inventory of ItemObjectBase instances.
 * Implements carrying capacity based on item weights and handles pickup/drop hooks.
 */
UCLASS(ClassGroup = "Inventory", meta = (BlueprintSpawnableComponent))
class NONAMED_API UNN_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNN_InventoryComponent();

	/** Check if an item can be added to the inventory */
	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool CanPickup(const UNN_ItemObjectBase* Item) const;

	/** Try to add an item to the inventory */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool PickupItem(UNN_ItemObjectBase* Item);

	/** Try to remove and drop an item from the inventory */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool DropItem(UNN_ItemObjectBase* Item);

	/** Get current total weight of all inventory items */
	UFUNCTION(BlueprintPure, Category = "Inventory")
	float GetWeight() const { return CurrentWeight; }

	/** Get maximum carrying weight capacity */
	UFUNCTION(BlueprintPure, Category = "Inventory")
	float GetMaxWeight() const { return MaxWeight; }

	/** Get all active items in the inventory */
	const TArray<TObjectPtr<UNN_ItemObjectBase>>& GetItems() const { return Items; }

protected:
	/** Hook called internally when an item is added */
	virtual void OnItemAdded(UNN_ItemObjectBase* Item);

	/** Hook called internally when an item is removed */
	virtual void OnItemRemoved(UNN_ItemObjectBase* Item);

	/** Recalculates total cached weight of all items */
	void RecalcWeight();

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UNN_ItemObjectBase>> Items;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (ClampMin = "0.0"))
	float MaxWeight = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float CurrentWeight = 0.0f;
};
