// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LootShooterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LOOTSHOOTER_API ULootShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "User Character", meta = (AllowPrivateAccess = "true"))
	class ALootShooterCharacter* LootShooterCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "User Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "User Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;
	UPROPERTY(BlueprintReadOnly, Category = "User Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
	UPROPERTY(BlueprintReadOnly, Category = "User Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category = "User Arm", meta = (AllowPrivateAccess = "true"))
	bool bIsWeaponEquipped;
	class AWeapon* EquipppedWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "User Arm", meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;

	UPROPERTY(BlueprintReadOnly, Category = "User Movement", meta = (AllowPrivateAccess = "true"))
	float YawOffset;
	UPROPERTY(BlueprintReadOnly, Category = "User Movement", meta = (AllowPrivateAccess = "true"))
	float CurrentLean;
	
	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;

	UPROPERTY(BlueprintReadOnly, Category = "User Movement", meta = (AllowPrivateAccess = "true"))
	float AO_Yaw;
	UPROPERTY(BlueprintReadOnly, Category = "User Movement", meta = (AllowPrivateAccess = "true"))
	float AO_Pitch;

	UPROPERTY(BlueprintReadOnly, Category = "User IK", meta = (AllowPrivateAccess = "true"))
	FTransform LeftHandTransform;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float _deltaTime) override;
};
