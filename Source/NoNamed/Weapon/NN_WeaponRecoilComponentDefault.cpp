#include "NN_WeaponRecoilComponentDefault.h"

UNN_WeaponRecoilComponentDefault::UNN_WeaponRecoilComponentDefault()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNN_WeaponRecoilComponentDefault::SetRecoilDef(const FNN_WeaponRecoilDef& InDef)
{
}

bool UNN_WeaponRecoilComponentDefault::HasRecoilDef() const
{
	return false;
}

FVector2D UNN_WeaponRecoilComponentDefault::CalcShotKick(int32 ShotIndex, bool bADS) const
{
	return FVector2D::ZeroVector;
}

FVector2D UNN_WeaponRecoilComponentDefault::CalcRecovery(float DeltaTime, const FVector2D& CurrentOffset) const
{
	return FVector2D::ZeroVector;
}

FVector2D UNN_WeaponRecoilComponentDefault::ClampOffset(const FVector2D& Offset) const
{
	return FVector2D::ZeroVector;
}

FVector2D UNN_WeaponRecoilComponentDefault::CalcOffsetAfterShot(int32 ShotIndex, bool bADS, const FVector2D& CurrentOffset) const
{
	return FVector2D::ZeroVector;
}

FVector2D UNN_WeaponRecoilComponentDefault::CalcOffsetAfterRecovery(float DeltaTime, const FVector2D& CurrentOffset) const
{
	return FVector2D::ZeroVector;
}
