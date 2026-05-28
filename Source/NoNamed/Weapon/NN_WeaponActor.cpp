#include "NN_WeaponActor.h"

#include "NN_WeaponDataAsset.h"
#include "NN_WeaponRecoilComponentDefault.h"
#include "NoNamed/Core/NN_ObjectBase.h"

ANN_WeaponActor::ANN_WeaponActor()
{
	RecoilComponent = CreateDefaultSubobject<UNN_WeaponRecoilComponentDefault>(TEXT("RecoilComponent"));
}

void ANN_WeaponActor::BindSourceObject(UNN_ObjectBase* InSourceObject)
{
	ANN_ItemActor::BindSourceObject(InSourceObject);
	RefreshRecoilComp();
}

void ANN_WeaponActor::RefreshRecoilComp()
{
	const UNN_WeaponDataAsset* WeaponData = GetWeaponData();
	if (!WeaponData || !WeaponData->bUseRecoil)
	{
		RemoveRecoilComp();
		return;
	}

	EnsureRecoilComp();
	RecoilComponent->SetRecoilDef(WeaponData->Recoil);
}

void ANN_WeaponActor::EnsureRecoilComp()
{
	if (RecoilComponent)
	{
		return;
	}

	RecoilComponent = NewObject<UNN_WeaponRecoilComponentDefault>(this, TEXT("RecoilComponent"));
	RecoilComponent->RegisterComponent();
}

void ANN_WeaponActor::RemoveRecoilComp()
{
	if (!RecoilComponent)
	{
		return;
	}

	RecoilComponent->DestroyComponent();
	RecoilComponent = nullptr;
}

const UNN_WeaponDataAsset* ANN_WeaponActor::GetWeaponData() const
{
	return Cast<UNN_WeaponDataAsset>(GetItemData());
}
