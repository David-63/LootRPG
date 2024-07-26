// Fill out your copyright notice in the Description page of Project Settings.


#include "LootShooterAnimInstance.h"
#include "LootShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	bIsInAir = LootShooterCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = LootShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bIsCrouched = LootShooterCharacter->bIsCrouched;
	bIsWeaponEquipped = LootShooterCharacter->IsWeaponEquipped();
	bIsAiming = LootShooterCharacter->IsAiming();

	// Offset yaw for strafing
	FRotator aimRotation = LootShooterCharacter->GetBaseAimRotation();
	FRotator movementRotation = UKismetMathLibrary::MakeRotFromX(LootShooterCharacter->GetVelocity());
	FRotator normalRotatorDelta = UKismetMathLibrary::NormalizedDeltaRotator(movementRotation, aimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, normalRotatorDelta, _deltaTime, 7.f);
	YawOffset = DeltaRotation.Yaw;

	// calulate CurrentLean
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = LootShooterCharacter->GetActorRotation();
	const FRotator rotateDelta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float changedRotation = rotateDelta.Yaw / _deltaTime;
	const float interpLean = FMath::FInterpTo(CurrentLean, changedRotation, _deltaTime, 5.f);
	CurrentLean = FMath::Clamp(interpLean, -90.f, 90.f);

}
