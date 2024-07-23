// Fill out your copyright notice in the Description page of Project Settings.


#include "LootShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "InputActionValue.h"
#include "enhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "LootShooter/Weapon/Weapon.h"
#include "LootShooter/ShooterComponents/CombatComponent.h"

ALootShooterCharacter::ALootShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 250.f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);

}

void ALootShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ALootShooterCharacter, OverlappingWeapon, COND_OwnerOnly);
}

void ALootShooterCharacter::BeginPlay()
{
	Super::BeginPlay();	
}
void ALootShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void ALootShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Add Input Mapping Context
	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* playerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			playerSubsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		enhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		enhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALootShooterCharacter::Move);
		enhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALootShooterCharacter::Look);
		enhancedInputComponent->BindAction(InteractiveAction, ETriggerEvent::Triggered, this, &ALootShooterCharacter::Interaction);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
void ALootShooterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat)
	{
		Combat->Character = this;
	}
}


void ALootShooterCharacter::Move(const FInputActionValue& _value)
{
	// input is a Vector2D
	FVector2D movementVector = _value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);

		// get forward vector
		const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(forwardDirection, movementVector.Y);
		AddMovementInput(rightDirection, movementVector.X);
	}
}
void ALootShooterCharacter::Look(const FInputActionValue& _value)
{
	// input is a Vector2D
	FVector2D lookAxisVector = _value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(lookAxisVector.X);
		AddControllerPitchInput(lookAxisVector.Y);
	}
}
void ALootShooterCharacter::Interaction(const FInputActionValue& _value)
{
	// ���߿� �÷��̾��� ��ȣ�ۿ� ������ ���� �ൿ�ϴ� ������ �޶��� ����
	if (Combat)
	{
		if (HasAuthority())
		{
			// ���������� �۵�
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			// Ŭ���̾�Ʈ������ �۵�
			ServerEquipButtonPressed();
		}
	}
}

void ALootShooterCharacter::ServerEquipButtonPressed_Implementation()
{
	// �������� Ŭ���̾�Ʈ�κ��� ��û���� �� ��ü(����)�� ������ �� ���⸦ ������ (Ŭ���̾�Ʈ�� ������� ���̶��� ����)
	if (Combat)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

void ALootShooterCharacter::SetOverlappingWeapon(AWeapon* _weapon)
{
	// ��Ƽ �÷��� ȯ�濡�� ȣ��ǹǷ�
	// �ϴ��� ������ ����
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = _weapon;
	// ���� �������� �÷��̾��� �����ֱ�
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

bool ALootShooterCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

void ALootShooterCharacter::OnRep_OverlappingWeapon(AWeapon* _lastWeapon)
{
	// ���������� ȣ��Ǵµ�
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (_lastWeapon)
	{
		_lastWeapon->ShowPickupWidget(false);
	}
}
