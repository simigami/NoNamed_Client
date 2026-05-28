#include "NN_ItemDataAsset.h"

#include "NN_ItemObjectBase.h"

UNN_ObjectBase* UNN_ItemDataAsset::CreateInstance(UObject* Outer)
{
	if (!Outer)
	{
		return nullptr;
	}

	TSubclassOf<UNN_ObjectBase> TargetClass = ObjectClass;
	if (!TargetClass)
	{
		TargetClass = UNN_ItemObjectBase::StaticClass();
	}

	UNN_ObjectBase* NewInstance = NewObject<UNN_ObjectBase>(Outer, TargetClass);
	if (NewInstance)
	{
		NewInstance->SetDataAsset(this);
	}

	return NewInstance;
}
