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
	bool IsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "User Movement", meta = (AllowPrivateAccess = "true"))
	bool IsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category = "User Movement", meta = (AllowPrivateAccess = "true"))
	bool IsCrouched;

	UPROPERTY(BlueprintReadOnly, Category = "User Arm", meta = (AllowPrivateAccess = "true"))
	bool IsWeaponEquipped;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float _deltaTime) override;
};
