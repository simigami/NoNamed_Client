#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NoNamed/Weapon/NN_WeaponDataAsset.h" // For ENN_WeaponClass
#include "NN_EquipmentComponent.generated.h"

class UNN_WeaponObjectBase;

/**
 * Component to manage equipped weapons for a character.
 * Handles specific slots: Primary (max 2), Secondary (max 1), Melee (max 1).
 */
UCLASS(ClassGroup = "Character", meta = (BlueprintSpawnableComponent))
class NONAMED_API UNN_EquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNN_EquipmentComponent();

	/** Checks if a weapon can be added to loadout slots (Primary/Secondary/Melee) */
	UFUNCTION(BlueprintPure, Category = "Equipment")
	bool CanEquipLoadout(const UNN_WeaponObjectBase* Weapon) const;

	/** Adds a weapon to its designated loadout slot (does NOT spawn physical actor) */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool EquipLoadout(UNN_WeaponObjectBase* Weapon);

	/** Removes a weapon from its designated loadout slot (destroys actor if in hand) */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool UnequipLoadout(UNN_WeaponObjectBase* Weapon);

	/** Truly holds the weapon in the character's hands (spawns actor and attaches it) */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool EquipHand(UNN_WeaponObjectBase* Weapon);

	/** Unequips / puts away the active weapon in hand (destroys its actor) */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void UnequipHand();

	/** Get the weapon currently being held in hand (Active Weapon) */
	UFUNCTION(BlueprintPure, Category = "Equipment")
	UNN_WeaponObjectBase* GetEquippedWeapon() const;

	/** Calculate total weight of all equipped slot weapons */
	float GetEquipmentWeight() const;

	/** Check if this component currently manages the weapon */
	UFUNCTION(BlueprintPure, Category = "Equipment")
	bool HasWeapon(const UNN_WeaponObjectBase* Weapon) const;

protected:
	void AttachWeaponActor(UNN_WeaponObjectBase* Weapon);
	void DetachWeaponActor(UNN_WeaponObjectBase* Weapon);

	UPROPERTY(VisibleAnywhere, Category = "Equipment")
	TArray<TObjectPtr<UNN_WeaponObjectBase>> Primaries;

	UPROPERTY(VisibleAnywhere, Category = "Equipment")
	TObjectPtr<UNN_WeaponObjectBase> Secondary;

	UPROPERTY(VisibleAnywhere, Category = "Equipment")
	TObjectPtr<UNN_WeaponObjectBase> Melee;

	UPROPERTY(VisibleAnywhere, Category = "Equipment")
	TObjectPtr<UNN_WeaponObjectBase> ActiveWeapon;
};
