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
	UE_LOG(LogTemp, Display, TEXT("aimRot: %f"), aimRotation.Yaw);
	FRotator movementRotation = UKismetMathLibrary::MakeRotFromX(LootShooterCharacter->GetVelocity());
	UE_LOG(LogTemp, Display, TEXT("actor rotation: %f"), movementRotation.Yaw);
	FRotator normalDeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(movementRotation, aimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, normalDeltaRotation, _deltaTime, 10.f);
	YawOffset = DeltaRotation.Yaw;
	UE_LOG(LogTemp, Display, TEXT("YawOffset : %f"), YawOffset);

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = LootShooterCharacter->GetActorRotation();
	const FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float target = delta.Yaw / _deltaTime;
	const float interp = FMath::FInterpTo(Lean, target, _deltaTime, 6.f);
	Lean = FMath::Clamp(interp, -90.f, 90.f);
}
