#pragma once

#include "CoreMinimal.h"
#include "NoNamed/Item/NN_ItemDataAsset.h"
#include "NN_WeaponDataAsset.generated.h"

/** Intrinsic weapon category (definition-time, not runtime loadout state) */
UENUM(BlueprintType)
enum class ENN_WeaponType : uint8
{
	None = 0,
	Pistol,
	SMG,
	Rifle,
	Sniper,
	Shotgun,
	LMG
};

UENUM(BlueprintType)
enum class ENN_WeaponClass : uint8
{
	None = 0,
	Primary,
	Secondary,
	Melee,
	Throw,
	Bomb,
	Etc,
};

/** How the weapon fires when the trigger is held or pressed */
UENUM(BlueprintType)
enum class ENN_WeaponFireMode : uint8
{
	None = 0,
	Single,
	SemiAuto,
	Auto,
};

/** Intrinsic recoil definition (degrees); not modified at runtime */
USTRUCT(BlueprintType)
struct FNN_WeaponRecoilDef
{
	GENERATED_BODY()

	/** Vertical view kick per shot (degrees, positive = look up) */
	UPROPERTY(EditAnywhere, Category = "Recoil", meta = (ClampMin = "0"))
	float VerticalPerShot = 0.35f;

	/** Random horizontal kick range per shot (degrees, symmetric) */
	UPROPERTY(EditAnywhere, Category = "Recoil", meta = (ClampMin = "0"))
	float HorizontalRange = 0.15f;

	/** How fast accumulated recoil returns toward zero (deg/sec) */
	UPROPERTY(EditAnywhere, Category = "Recoil", meta = (ClampMin = "0"))
	float RecoverySpeed = 8.0f;

	/** Max accumulated pitch recoil (degrees) */
	UPROPERTY(EditAnywhere, Category = "Recoil", meta = (ClampMin = "0"))
	float MaxPitch = 15.0f;

	/** Max accumulated yaw recoil (degrees) */
	UPROPERTY(EditAnywhere, Category = "Recoil", meta = (ClampMin = "0"))
	float MaxYaw = 8.0f;

	/** Multiplier applied to the first shot in a burst / spray */
	UPROPERTY(EditAnywhere, Category = "Recoil", meta = (ClampMin = "0"))
	float FirstShotScale = 1.0f;

	/** Recoil scale while aiming down sights (0-1 typical) */
	UPROPERTY(EditAnywhere, Category = "Recoil", meta = (ClampMin = "0", ClampMax = "2"))
	float ADSScale = 0.65f;
};

/**
 * DataAsset for weapon definitions.
 * Holds intrinsic FPS-style stats that do not change during a match (CS2 / Apex style).
 */
UCLASS()
class NONAMED_API UNN_WeaponDataAsset : public UNN_ItemDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Identity")
	ENN_WeaponType WeaponType = ENN_WeaponType::Rifle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Identity")
	ENN_WeaponClass WeaponClass = ENN_WeaponClass::Primary;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Identity")
	ENN_WeaponFireMode FireMode = ENN_WeaponFireMode::Auto;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Identity")
	FName AttachSocketName;

	/** Base damage per shot (or per pellet for shotguns) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Damage", meta = (ClampMin = "0"))
	float Damage = 30.0f;

	/** Headshot damage multiplier (e.g. 4.0 = 400% on head) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Damage", meta = (ClampMin = "1"))
	float HeadshotMultiplier = 2.0f;

	/** Rounds per minute */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire", meta = (ClampMin = "1"))
	float FireRateRPM = 600.0f;

	/** Shots per burst when FireMode is Semi-Auto */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire", meta = (ClampMin = "1", EditCondition = "FireMode == ENN_WeaponFireMode::SemiAuto"))
	int32 SemiAutoCount = 3;
	
	/** Shots per burst when FireMode is Semi-Auto */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire", meta = (ClampMin = "1", EditCondition = "FireMode == ENN_WeaponFireMode::Auto"))
	int32 AutoCount = MagazineSize;

	/** Rounds per magazine / clip */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Ammo", meta = (ClampMin = "1"))
	int32 MagazineSize = 30;

	/** Maximum reserve ammo the weapon can carry */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Ammo", meta = (ClampMin = "0"))
	int32 MaxReserveAmmo = 90;

	/** Full reload duration in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Ammo", meta = (ClampMin = "0.01"))
	float ReloadTime = 2.5f;

	/** Pellets per trigger pull (shotguns); 1 for hitscan rifles */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Fire", meta = (ClampMin = "1"))
	int32 PelletsPerShot = 1;

	/** False for weapons with no view kick (e.g. molotov, grenade) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Recoil")
	bool bUseRecoil = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Recoil", meta = (EditCondition = "bUseRecoil", EditConditionHides))
	FNN_WeaponRecoilDef Recoil;

	virtual UNN_ObjectBase* CreateInstance(UObject* Outer) override;
};
