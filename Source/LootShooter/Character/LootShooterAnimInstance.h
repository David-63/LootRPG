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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User Character", meta = (AllowPrivateAccess = "true"))
	class ALootShooterCharacter* LootShooterCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User Movement", meta = (AllowPrivateAccess = "true"))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User Movement", meta = (AllowPrivateAccess = "true"))
	bool IsAccelerating;


public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float _deltaTime) override;
};
