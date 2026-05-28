#pragma once

#include "CoreMinimal.h"
#include "NoNamed/Data/NN_DataAsset.h"
#include "NN_ItemDataAsset.generated.h"

/**
 * Parent DataAsset for all obtainable in-game items (weapons, consumables, gear, etc.).
 */
UCLASS()
class NONAMED_API UNN_ItemDataAsset : public UNN_DataAsset
{
	GENERATED_BODY()

public:
	/** Inventory / loadout weight cost for one unit of this item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (ClampMin = "0"))
	float Weight = 1.0f;

	virtual UNN_ObjectBase* CreateInstance(UObject* Outer) override;
};
