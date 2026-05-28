#include "NN_WeaponDataAsset.h"

#include "NN_WeaponObjectBase.h"

UNN_ObjectBase* UNN_WeaponDataAsset::CreateInstance(UObject* Outer)
{
	if (!Outer)
	{
		return nullptr;
	}

	TSubclassOf<UNN_ObjectBase> TargetClass = ObjectClass;
	if (!TargetClass)
	{
		TargetClass = UNN_WeaponObjectBase::StaticClass();
	}

	UNN_ObjectBase* NewInstance = NewObject<UNN_ObjectBase>(Outer, TargetClass);
	if (NewInstance)
	{
		NewInstance->SetDataAsset(this);
	}

	return NewInstance;
}
