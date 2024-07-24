// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOOTSHOOTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class ALootShooterCharacter;
private:
	class ALootShooterCharacter* Character;
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;
	UPROPERTY(Replicated)
	bool bIsAiming;
	

protected:
	void SetAiming(bool _bIsAiming);
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool _bIsAiming);
	UFUNCTION()
	void OnRep_EquippedWeapon();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& _outLifetimeProps) const override;
	void EquipWeapon(AWeapon* _weaponToEquip);

public:
	UCombatComponent();
protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
