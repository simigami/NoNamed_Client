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
	
	UFUNCTION(BlueprintPure, Category = "Weapon")
	ENN_WeaponFireMode GetWeaponFireMode() const;
	
	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetWeaponSemiAutoCount() const;

	// TODO: Reload related function should go protected, and only invoke TryReload in outside (Character...)
	void TryReload();
	
	//~ Begin INN_CombatWeaponInterface interface
	virtual bool CanFire() const override;
	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual bool CanReload() const override;
	virtual void Reload() override;
	virtual void FinishReload() override;;
	//~ End INN_CombatWeaponInterface interface

protected:
	void FireShot();
	void TryFireShot();
	void ExecuteLineTrace();
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon|State")
	bool bCanFireWeapon = true;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon|State")
	bool bCanReload = true;

	UPROPERTY(VisibleAnywhere, Category = "Weapon|State")
	int32 MaxReserveAmmo = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon|State")
	int32 CurrentAmmo = 0;

	UPROPERTY(VisibleAnywhere, Category = "Weapon|State")
	uint8 SemiAutoShotCount = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon|State")
	double LastFireTime = 0.0;

	/** World time when StartFire was called (-1 = not firing) */
	double FirePressTime = -1.0;

	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadTimerHandle;
};
