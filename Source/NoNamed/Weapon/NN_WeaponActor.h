#pragma once

#include "CoreMinimal.h"
#include "NoNamed/Item/NN_ItemActor.h"
#include "NN_WeaponActor.generated.h"

class UNN_WeaponDataAsset;
class UNN_WeaponRecoilComponentDefault;

/**
 * World representation actor for a weapon.
 */
UCLASS()
class NONAMED_API ANN_WeaponActor : public ANN_ItemActor
{
	GENERATED_BODY()

public:
	ANN_WeaponActor();

	/** Null when the weapon DataAsset has bUseRecoil == false */
	UNN_WeaponRecoilComponentDefault* GetRecoilComp() const { return RecoilComponent; }

	virtual void BindSourceObject(UNN_ObjectBase* InSourceObject) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon|Recoil")
	TObjectPtr<UNN_WeaponRecoilComponentDefault> RecoilComponent;

	void RefreshRecoilComp();
	void EnsureRecoilComp();
	void RemoveRecoilComp();

	const UNN_WeaponDataAsset* GetWeaponData() const;
};
