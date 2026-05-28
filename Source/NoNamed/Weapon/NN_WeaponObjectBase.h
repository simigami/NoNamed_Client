#pragma once

#include "CoreMinimal.h"
#include "NoNamed/Item/NN_ItemObjectBase.h"
#include "NoNamed/Weapon/NN_WeaponDataAsset.h" // For ENN_WeaponClass
#include "NoNamed/Interface/NN_CombatWeaponInterface.h"
#include "NN_WeaponObjectBase.generated.h"

/**
 * Base UObject logic instance for weapons.
 */
UCLASS(Blueprintable)
class NONAMED_API UNN_WeaponObjectBase : public UNN_ItemObjectBase, public INN_CombatWeaponInterface
{
	GENERATED_BODY()

public:
	virtual void SetDataAsset(UNN_DataAsset* InDataAsset) override;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	ENN_WeaponClass GetWeaponClass() const;

	//~ Begin INN_CombatWeaponInterface interface
	virtual bool CanFire() const override;
	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual bool CanReload() const override;
	virtual void Reload() override;
	//~ End INN_CombatWeaponInterface interface

protected:
	void FireShot();
	void ExecuteLineTrace();

	UPROPERTY(VisibleAnywhere, Category = "Weapon|State")
	int32 CurrentAmmo = 0;

	UPROPERTY(VisibleAnywhere, Category = "Weapon|State")
	double LastFireTime = 0.0;

	/** World time when StartFire was called (-1 = not firing) */
	double FirePressTime = -1.0;

	FTimerHandle FireTimerHandle;
};
