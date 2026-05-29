#include "NN_WeaponObjectBase.h"
#include "NN_WeaponDataAsset.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

void UNN_WeaponObjectBase::SetDataAsset(UNN_DataAsset* InDataAsset)
{
	Super::SetDataAsset(InDataAsset);
	
	if (const UNN_WeaponDataAsset* WeaponData = Cast<UNN_WeaponDataAsset>(InDataAsset))
	{
		CurrentAmmo = WeaponData->MagazineSize;
		MaxReserveAmmo = WeaponData->MaxReserveAmmo;
	}
}

ENN_WeaponClass UNN_WeaponObjectBase::GetWeaponClass() const
{
	const UNN_WeaponDataAsset* WeaponData = Cast<UNN_WeaponDataAsset>(DataAsset);
	return WeaponData ? WeaponData->WeaponClass : ENN_WeaponClass::None;
}

ENN_WeaponFireMode UNN_WeaponObjectBase::GetWeaponFireMode() const
{
	const UNN_WeaponDataAsset* WeaponData = Cast<UNN_WeaponDataAsset>(DataAsset);
	return WeaponData ? WeaponData->FireMode : ENN_WeaponFireMode::None;
}

int32 UNN_WeaponObjectBase::GetWeaponSemiAutoCount() const
{
	const UNN_WeaponDataAsset* WeaponData = Cast<UNN_WeaponDataAsset>(DataAsset);
	if (WeaponData && WeaponData->FireMode == ENN_WeaponFireMode::SemiAuto)
	{
		return WeaponData->SemiAutoCount;
	}
	
	return 0;
}

bool UNN_WeaponObjectBase::CanFire() const
{
	if (!bCanFireWeapon)
	{
		return false;
	}

	// TODO: IOCP, Move authoritative fire-state validation to the server.
	if (CurrentAmmo <= 0)
	{
		return false;
	}

	const UNN_WeaponDataAsset* WeaponData = Cast<UNN_WeaponDataAsset>(DataAsset);
	if (!WeaponData || WeaponData->FireRateRPM <= 0.0f)
	{
		return false;
	}

	const UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	// TODO: IOCP, Server owns fire cadence; client RPM gate is temporary prediction only.
	const double CurrentTime = World->GetTimeSeconds();
	const double Cooldown = 60.0 / WeaponData->FireRateRPM;
	if (CurrentTime - LastFireTime < Cooldown)
	{
		return false;
	}

	if (GetWeaponFireMode() == ENN_WeaponFireMode::SemiAuto)
	{
		if (SemiAutoShotCount >= GetWeaponSemiAutoCount())
		{
			return false;
		}
	}

	return true;
}

void UNN_WeaponObjectBase::StartFire()
{
	bCanFireWeapon = true;
	
	// TODO: IOCP, Send fire-pressed input to the server instead of authoritatively firing here.
	const UNN_WeaponDataAsset* WeaponData = Cast<UNN_WeaponDataAsset>(DataAsset);
	if (!WeaponData)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FirePressTime = World->GetTimeSeconds();
	UE_LOG(LogTemp, Log, TEXT("UNN_WeaponObjectBase::StartFire - Fire started. Magazine: %d, Reserve: %d"), CurrentAmmo, MaxReserveAmmo);

	TryFireShot();

	if (WeaponData->FireMode != ENN_WeaponFireMode::Single)
	{
		// TODO: IOCP, Replace timer-driven auto fire with server-authoritative fire cadence.
		const float Interval = 60.0f / WeaponData->FireRateRPM;
		World->GetTimerManager().SetTimer(FireTimerHandle, this, &UNN_WeaponObjectBase::TryFireShot, Interval, true);
	}
}

void UNN_WeaponObjectBase::StopFire()
{
	bCanFireWeapon = false;
	
	// TODO: IOCP, Send fire-released input to the server.
	if (GetWeaponFireMode() == ENN_WeaponFireMode::SemiAuto)
	{
		SemiAutoShotCount = 0;
	}

	FirePressTime = -1.0;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(FireTimerHandle);
	}
}

bool UNN_WeaponObjectBase::CanReload() const
{
	// TODO: IOCP, Validate reload state on the server.
	if (!bCanReload)
	{
		return false;
	}
	
	if (MaxReserveAmmo == 0)
	{
		return false;
	}
	
	if (const UNN_WeaponDataAsset* WeaponData = Cast<UNN_WeaponDataAsset>(DataAsset))
	{
		if (CurrentAmmo == WeaponData->MagazineSize)
		{
			return false;
		}
	}

	return true;
}

void UNN_WeaponObjectBase::FinishReload()
{
	if (const UNN_WeaponDataAsset* WeaponData = Cast<UNN_WeaponDataAsset>(DataAsset))
	{
		const int32 AmmoToFullMagazine = WeaponData->MagazineSize - CurrentAmmo;
		const int32 NextAmmo = MaxReserveAmmo >= AmmoToFullMagazine 
		? AmmoToFullMagazine 
		: MaxReserveAmmo;
		
		CurrentAmmo += NextAmmo;
		MaxReserveAmmo -= NextAmmo;
		
		bCanReload = true;
		bCanFireWeapon = true;

		UE_LOG(LogTemp, Log, TEXT("UNN_WeaponObjectBase::FinishReload - Reload complete. Loaded: %d, Magazine: %d, Reserve: %d"),
			NextAmmo, CurrentAmmo, MaxReserveAmmo);
	}
}

void UNN_WeaponObjectBase::Reload()
{
	// TODO: IOCP, Implement authoritative reload timing and ammo transfer on the server.
	if (const UNN_WeaponDataAsset* WeaponData = Cast<UNN_WeaponDataAsset>(DataAsset))
	{
		StopFire();
		
		const float ReloadTime = WeaponData->ReloadTime;
		
		bCanReload = false;
		bCanFireWeapon = false;

		UE_LOG(LogTemp, Log, TEXT("UNN_WeaponObjectBase::Reload - Reload started (%.2fs). Magazine: %d, Reserve: %d"),
			ReloadTime, CurrentAmmo, MaxReserveAmmo);
		
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &UNN_WeaponObjectBase::FinishReload, ReloadTime, false);
	}
}

void UNN_WeaponObjectBase::FireShot()
{
	// TODO: IOCP, Server should own ammo consumption and shot creation.
	const UNN_WeaponDataAsset* WeaponData = Cast<UNN_WeaponDataAsset>(DataAsset);
	if (CurrentAmmo <= 0 || !WeaponData || WeaponData->FireRateRPM <= 0.0f)
	{
		StopFire();
		return;
	}

	const UWorld* World = GetWorld();
	const double CurrentTime = World ? World->GetTimeSeconds() : 0.0;
	const double Cooldown = 60.0 / WeaponData->FireRateRPM;
	if (World && CurrentTime - LastFireTime < Cooldown)
	{
		// TODO: IOCP, Server owns fire cadence; skip this tick without ending hold-to-fire.
		return;
	}
	
	if (GetWeaponFireMode() == ENN_WeaponFireMode::SemiAuto)
	{
		++SemiAutoShotCount;
	}

	CurrentAmmo--;
	LastFireTime = CurrentTime;
	UE_LOG(LogTemp, Log, TEXT("UNN_WeaponObjectBase::FireShot - Fired. Magazine: %d, Reserve: %d"), CurrentAmmo, MaxReserveAmmo);
	ExecuteLineTrace();
}

void UNN_WeaponObjectBase::TryFireShot()
{
	if (!bCanFireWeapon)
	{
		StopFire();
		return;
	}

	if (CurrentAmmo <= 0)
	{
		StopFire();
		return;
	}

	const UNN_WeaponDataAsset* WeaponData = Cast<UNN_WeaponDataAsset>(DataAsset);
	if (!WeaponData || WeaponData->FireRateRPM <= 0.0f)
	{
		StopFire();
		return;
	}

	if (GetWeaponFireMode() == ENN_WeaponFireMode::SemiAuto)
	{
		if (SemiAutoShotCount >= GetWeaponSemiAutoCount())
		{
			StopFire();
			return;
		}
	}

	// TODO: IOCP, Server owns fire cadence and shot authorization; FireShot applies temporary client RPM gate.
	FireShot();
}

void UNN_WeaponObjectBase::TryReload()
{
	if (!CanReload())
	{
		return;
	}
	
	Reload();
}

void UNN_WeaponObjectBase::ExecuteLineTrace()
{
	// TODO: IOCP, Replace local hit detection with server-authoritative trace validation.
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FVector Start = FVector::ZeroVector;
	FVector End = FVector::ZeroVector;
	FVector TraceDir = FVector::ForwardVector;

	ACharacter* Character = Cast<ACharacter>(GetOuter());
	if (Character)
	{
		if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
		{
			FRotator CameraRotation;
			PC->GetPlayerViewPoint(Start, CameraRotation);
			TraceDir = CameraRotation.Vector();
			End = Start + (TraceDir * 5000.0f);
		}
	}

	// Fallback to muzzle actor if no character or controller viewpoint found
	if (Start.IsNearlyZero() && SpawnedActor)
	{
		Start = SpawnedActor->GetActorLocation();
		TraceDir = SpawnedActor->GetActorForwardVector();
		End = Start + (TraceDir * 5000.0f);
	}

	FHitResult Hit;
	FCollisionQueryParams Params;
	if (SpawnedActor)
	{
		Params.AddIgnoredActor(SpawnedActor);
		if (SpawnedActor->GetOwner())
		{
			Params.AddIgnoredActor(SpawnedActor->GetOwner());
		}
	}
	if (Character)
	{
		Params.AddIgnoredActor(Character);
	}

	World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	DrawDebugLine(World, Start, Hit.bBlockingHit ? Hit.ImpactPoint : End, FColor::Red, false, 0.1f, 0, 2.0f);
}
