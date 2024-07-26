// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LootShooterCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class LOOTSHOOTER_API ALootShooterCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "User Camera")
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, Category = "User Camera")
	class UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;
		
	/*
	* Input Action
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractiveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	/*
	* weapon & combat
	*/
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;
	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;


	float AO_Yaw;
	float AO_Pitch;
	FRotator StartAimRotation;

private:
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* _lastWeapon);
	// RPC (Remote Procedure Call) Ŭ���̾�Ʈ���� ������ ȣ���ϴ� �Լ�
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

protected:
	void AimOffset(float _deltaTime);

public:
	virtual void PostInitializeComponents() override;
	
	// Replicated ������ ����Ϸ��� �ݵ�� ����������
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& _outLifetimeProps) const override;
	// Weapon�� ���� ȣ���
	void SetOverlappingWeapon(AWeapon* _weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }

public:
	ALootShooterCharacter();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Called for movement input */
	void InputMove(const FInputActionValue& _value);
	/** Called for looking input */
	void InputLook(const FInputActionValue& _value);
	/** Called for Interaction input */
	void InputInteraction(const FInputActionValue& _value);
	void InputCrouch(const FInputActionValue& _value);
	void InputAim(const FInputActionValue& _value);
};
