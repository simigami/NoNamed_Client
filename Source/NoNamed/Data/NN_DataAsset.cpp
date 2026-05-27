#include "NN_DataAsset.h"
#include "NoNamed/Core/NN_ObjectBase.h"

FNN_ObjectSpawnContext UNN_DataAsset::BuildSpawnContext() const
{
	FNN_ObjectSpawnContext Context;
	Context.Identity = Identity;
	Context.Mesh = Mesh;
	return Context;
}

UNN_ObjectBase* UNN_DataAsset::CreateInstance(UObject* Outer)
{
	if (!Outer)
	{
		return nullptr;
	}

	TSubclassOf<UNN_ObjectBase> TargetClass = ObjectClass;
	if (!TargetClass)
	{
		TargetClass = UNN_ObjectBase::StaticClass();
	}

	UNN_ObjectBase* NewObjectInstance = NewObject<UNN_ObjectBase>(Outer, TargetClass);
	if (NewObjectInstance)
	{
		NewObjectInstance->SetDataAsset(this);
	}

	return NewObjectInstance;
}
