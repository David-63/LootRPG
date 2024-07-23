// Fill out your copyright notice in the Description page of Project Settings.


#include "LootShooterAnimInstance.h"
#include "LootShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void ULootShooterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	LootShooterCharacter = Cast<ALootShooterCharacter>(TryGetPawnOwner());
}

void ULootShooterAnimInstance::NativeUpdateAnimation(float _deltaTime)
{
	Super::NativeUpdateAnimation(_deltaTime);

	if (nullptr == LootShooterCharacter)
	{
		LootShooterCharacter = Cast<ALootShooterCharacter>(TryGetPawnOwner());
	}

	if (nullptr == LootShooterCharacter) return;

	FVector velocity = LootShooterCharacter->GetVelocity();
	velocity.Z = 0.f;
	MoveSpeed = velocity.Size();
	IsInAir = LootShooterCharacter->GetCharacterMovement()->IsFalling();
	IsAccelerating = LootShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	IsWeaponEquipped = LootShooterCharacter->IsWeaponEquipped();
}
