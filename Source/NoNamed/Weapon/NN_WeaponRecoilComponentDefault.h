#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NN_WeaponDataAsset.h"
#include "NN_WeaponRecoilComponentDefault.generated.h"

/**
 * Default weapon recoil controller.
 * Pure C++ math helpers; static tuning comes from FNN_WeaponRecoilDef on the DataAsset.
 */
UCLASS(ClassGroup = "Weapon", meta = (BlueprintSpawnableComponent))
class NONAMED_API UNN_WeaponRecoilComponentDefault : public UActorComponent
{
	GENERATED_BODY()

public:
	UNN_WeaponRecoilComponentDefault();

	void SetRecoilDef(const FNN_WeaponRecoilDef& InDef);
	bool HasRecoilDef() const;

	/** View kick for a single shot (X = pitch, Y = yaw), degrees */
	FVector2D CalcShotKick(int32 ShotIndex, bool bADS) const;

	/** Recoil removed by recovery this tick (X = pitch, Y = yaw), degrees */
	FVector2D CalcRecovery(float DeltaTime, const FVector2D& CurrentOffset) const;

	/** Clamp accumulated recoil to weapon limits */
	FVector2D ClampOffset(const FVector2D& Offset) const;

	/** Apply one shot kick onto an existing offset, then clamp */
	FVector2D CalcOffsetAfterShot(int32 ShotIndex, bool bADS, const FVector2D& CurrentOffset) const;

	/** Offset after recovery step, then clamp */
	FVector2D CalcOffsetAfterRecovery(float DeltaTime, const FVector2D& CurrentOffset) const;

protected:
	FNN_WeaponRecoilDef RecoilDef;
	bool bHasDef = false;
};
