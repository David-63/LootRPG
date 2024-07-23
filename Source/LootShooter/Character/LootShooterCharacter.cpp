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

// Sets default values
ALootShooterCharacter::ALootShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 250.f;
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

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
	// 나중에 플레이어의 상호작용 변수에 따라 행동하는 변수가 달라질 예정
	if (Combat)
	{
		if (HasAuthority())
		{
			// 서버에서만 작동
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			// 클라이언트에서만 작동
			ServerEquipButtonPressed();
		}
	}
}

void ALootShooterCharacter::ServerEquipButtonPressed_Implementation()
{
	if (Combat)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

void ALootShooterCharacter::SetOverlappingWeapon(AWeapon* _weapon)
{
	// 멀티 플레이 환경에서 호출되므로
	// 일단은 위젯을 끄고
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = _weapon;
	// 폰을 제어중인 플레이어라면 보여주기
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

void ALootShooterCharacter::OnRep_OverlappingWeapon(AWeapon* _lastWeapon)
{
	// 리슨서버만 호출되는듯
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (_lastWeapon)
	{
		_lastWeapon->ShowPickupWidget(false);
	}
}
