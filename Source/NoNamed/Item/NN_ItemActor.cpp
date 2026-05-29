#include "NN_ItemActor.h"

#include "NN_ItemDataAsset.h"
#include "NoNamed/Core/NN_ObjectBase.h"

ANN_ItemActor::ANN_ItemActor()
{
}

const UNN_ItemDataAsset* ANN_ItemActor::GetItemData() const
{
	if (!SourceObject || !SourceObject->DataAsset)
	{
		return nullptr;
	}

	return Cast<UNN_ItemDataAsset>(SourceObject->DataAsset);
}

float ANN_ItemActor::GetItemWeight() const
{
	const UNN_ItemDataAsset* ItemData = GetItemData();
	return ItemData ? ItemData->Weight : 0.0f;
}
