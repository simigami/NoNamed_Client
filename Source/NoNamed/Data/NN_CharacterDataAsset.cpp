// Fill out your copyright notice in the Description page of Project Settings.


#include "NN_CharacterDataAsset.h"

#include "NoNamed/Character/NN_CharacterObjectBase.h"

UNN_ObjectBase* UNN_CharacterDataAsset::CreateInstance(UObject* Outer)
{
	if (!Outer)
	{
		return nullptr;
	}

	TSubclassOf<UNN_ObjectBase> TargetClass = ObjectClass;
	if (!TargetClass)
	{
		TargetClass = UNN_CharacterObjectBase::StaticClass();
	}

	UNN_ObjectBase* NewObjectInstance = NewObject<UNN_ObjectBase>(Outer, TargetClass);
	if (NewObjectInstance)
	{
		NewObjectInstance->SetDataAsset(this);
	}

	return NewObjectInstance;
}
