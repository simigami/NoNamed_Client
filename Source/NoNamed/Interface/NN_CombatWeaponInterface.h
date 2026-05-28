#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NN_CombatWeaponInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UNN_CombatWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for combat weapon interactions (firing, reloading).
 */
class NONAMED_API INN_CombatWeaponInterface
{
	GENERATED_BODY()

public:
	/** Checks if the weapon is in a state where it can fire */
	virtual bool CanFire() const;

	/** Starts the firing sequence */
	virtual void StartFire();

	/** Stops the firing sequence */
	virtual void StopFire();

	/** Checks if the weapon can be reloaded */
	virtual bool CanReload() const;

	/** Executes the reload sequence */
	virtual void Reload();
};
