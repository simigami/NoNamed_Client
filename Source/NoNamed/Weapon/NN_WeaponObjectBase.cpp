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
	}
}

ENN_WeaponClass UNN_WeaponObjectBase::GetWeaponClass() const
{
	const UNN_WeaponDataAsset* WeaponData = Cast<UNN_WeaponDataAsset>(DataAsset);
	return WeaponData ? WeaponData->WeaponClass : ENN_WeaponClass::None;
}

bool UNN_WeaponObjectBase::CanFire() const
{
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

	const double CurrentTime = GetWorld()->GetTimeSeconds();
	const double Cooldown = 60.0 / WeaponData->FireRateRPM;
	if (CurrentTime - LastFireTime < Cooldown)
	{
		return false;
	}

	return true;
}

void UNN_WeaponObjectBase::StartFire()
{
	// TODO: IOCP, Send fire-pressed input to the server instead of authoritatively firing here.
	const UNN_WeaponDataAsset* WeaponData = Cast<UNN_WeaponDataAsset>(DataAsset);
	if (!WeaponData)
	{
		return;
	}

	FirePressTime = GetWorld()->GetTimeSeconds();
	UE_LOG(LogTemp, Log, TEXT("UNN_WeaponObjectBase::StartFire - Pressed at %.3f"), FirePressTime);

	if (CanFire())
	{
		FireShot();
	}

	if (WeaponData->FireMode == ENN_WeaponFireMode::Auto)
	{
		// TODO: IOCP, Replace timer-driven auto fire with a NextFireTime scheduler so RPM is not limited by timer drift.
		const float Interval = 60.0f / WeaponData->FireRateRPM;
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UNN_WeaponObjectBase::FireShot, Interval, true);
	}
}

void UNN_WeaponObjectBase::StopFire()
{
	// TODO: IOCP, Send fire-released input to the server.
	const double Now = GetWorld()->GetTimeSeconds();
	if (FirePressTime >= 0.0)
	{
		UE_LOG(LogTemp, Log, TEXT("UNN_WeaponObjectBase::StopFire - Released at %.3f, held %.3f sec"),
			Now, Now - FirePressTime);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UNN_WeaponObjectBase::StopFire - Released at %.3f"), Now);
	}

	FirePressTime = -1.0;
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

bool UNN_WeaponObjectBase::CanReload() const
{
	// TODO: IOCP, Validate reload state on the server.
	return true;
}

void UNN_WeaponObjectBase::Reload()
{
	// TODO: IOCP, Implement authoritative reload timing and ammo transfer on the server.
	UE_LOG(LogTemp, Log, TEXT("UNN_WeaponObjectBase::Reload - Weapon reload trigger."));
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

	const double CurrentTime = GetWorld()->GetTimeSeconds();
	const double Cooldown = 60.0 / WeaponData->FireRateRPM;
	if (CurrentTime - LastFireTime < Cooldown)
	{
		// TODO: IOCP, Move RPM gating out of FireShot; cooldown checks here can skip timer ticks and lower the effective RPM.
		return;
	}

	CurrentAmmo--;
	LastFireTime = CurrentTime;
	ExecuteLineTrace();
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

	// Draw visible red debug line representing the camera look direction
	DrawDebugLine(World, Start, Hit.bBlockingHit ? Hit.ImpactPoint : End, FColor::Red, false, 0.1f, 0, 2.0f);
	
	UE_LOG(LogTemp, Log, TEXT("UNN_WeaponObjectBase::ExecuteLineTrace - Fired shot in camera direction. Ammo: %d"), CurrentAmmo);
}
