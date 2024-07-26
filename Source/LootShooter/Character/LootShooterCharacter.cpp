// Fill out your copyright notice in the Description page of Project Settings.


#include "LootShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "InputActionValue.h"
#include "enhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "LootShooter/Weapon/Weapon.h"
#include "LootShooter/ShooterComponents/CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"

ALootShooterCharacter::ALootShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 65.f; // The camera follows at this distance behind the character
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

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
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
	AimOffset(DeltaTime);
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
		enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALootShooterCharacter::InputMove);
		enhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALootShooterCharacter::InputLook);
		enhancedInputComponent->BindAction(InteractiveAction, ETriggerEvent::Triggered, this, &ALootShooterCharacter::InputInteraction);
		enhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ALootShooterCharacter::InputCrouch);
		enhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &ALootShooterCharacter::InputAim);
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


void ALootShooterCharacter::InputMove(const FInputActionValue& _value)
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
void ALootShooterCharacter::InputLook(const FInputActionValue& _value)
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
void ALootShooterCharacter::InputInteraction(const FInputActionValue& _value)
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

void ALootShooterCharacter::InputCrouch(const FInputActionValue& _value)
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void ALootShooterCharacter::InputAim(const FInputActionValue& _value)
{
	if (Combat)
	{
		Combat->SetAiming(_value.Get<bool>());
	}
}

void ALootShooterCharacter::ServerEquipButtonPressed_Implementation()
{
	// 서버에서 클라이언트로부터 요청받은 폰 객체(서버)에 오버랩 된 무기를 부착함 (클라이언트가 복사받은 폰이랑은 무관)
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

bool ALootShooterCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

bool ALootShooterCharacter::IsAiming()
{
	return (Combat && Combat->bIsAiming);
}

void ALootShooterCharacter::AimOffset(float _deltaTime)
{
	if (Combat && nullptr == Combat->EquippedWeapon) return;

	FVector velocity = GetVelocity();
	velocity.Z = 0.f;
	float moveSpeed = velocity.Size();
	bool isInAir = GetCharacterMovement()->IsFalling();

	if (0.f == moveSpeed && !isInAir) // standing still, not jumping
	{
		FRotator currentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator deltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(currentAimRotation, StartAimRotation);
		AO_Yaw = deltaAimRotation.Yaw;
		bUseControllerRotationYaw = false;
	}
	if (0.f < moveSpeed || isInAir) // running, or jumping
	{
		StartAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
	}
	
	AO_Pitch = GetBaseAimRotation().Pitch;
	// 멀티환경에서 압축된 회전값을 복원함
	if (90.f < AO_Pitch && !IsLocallyControlled())
	{
		// map pitch from [270, 360) to [-90, 0)
		FVector2D inRange(270.f, 360.f);
		FVector2D outRange(-90, 0);
		AO_Pitch = FMath::GetMappedRangeValueClamped(inRange, outRange, AO_Pitch);
	}
}