#pragma once

#include "CoreMinimal.h"
#include "NoNamed/Core/NN_ActorBase.h"
#include "NN_ItemActor.generated.h"

class UNN_ItemDataAsset;

/**
 * World representation actor for a generic obtainable item.
 */
UCLASS()
class NONAMED_API ANN_ItemActor : public ANN_ActorBase
{
	GENERATED_BODY()

public:
	ANN_ItemActor();

	const UNN_ItemDataAsset* GetItemData() const;
	float GetItemWeight() const;
};
