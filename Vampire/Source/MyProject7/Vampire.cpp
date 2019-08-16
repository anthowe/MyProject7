// Fill out your copyright notice in the Description page of Project Settings.

#include "Vampire.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"
#include "Animation/AnimInstance.h"


// Sets default values
AVampire::AVampire()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 840.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 350.f;
	GetCharacterMovement()->AirControl = 0.2f;

	bIsAttacking = false;
	bShiftKeyDown = false;
	bLMBDown = false;

	MaxHealth = 1000.f;
	Health = 65.f;
	MaxStamina = 150.f;
	Stamina = 120.f;
	Coins = 0;

	bMovingForward = false;
	bMovingRight = false;
	RunningSpeed = 650.f;
	SprintingSpeed = 950.f;

	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;
}

// Called when the game starts or when spawned
void AVampire::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AVampire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:

		if (bShiftKeyDown)
		{
			if (Stamina - DeltaStamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
				Stamina -= DeltaStamina;
			}
			else
			{
				Stamina -= DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		else
		{
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}

		break;
	case EStaminaStatus::ESS_BelowMinimum:

		if (bShiftKeyDown)
		{
			if (Stamina - DeltaStamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else
			{
				Stamina -= DeltaStamina;

				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}

		}
		else
		{
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown)
		{
			Stamina = 0.f;
		}
		else
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);

			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);

		break;

	case EStaminaStatus::ESS_ExhaustedRecovering:

		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);

		break;

	default:
		;
	}
}

// Called to bind functionality to input
void AVampire::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AVampire::MoveForward);
	PlayerInputComponent->BindAxis("MoveRIght", this, &AVampire::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AVampire::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AVampire::LMBUp);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AVampire::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AVampire::ShiftKeyUp);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

}

void AVampire::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

UE_LOG(LogTemp, Warning, TEXT("MoveForward called"));
	}

}

void AVampire::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}

}

void AVampire::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AVampire::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AVampire::DecrementHealth(float Amount)
{
	if (Health - Amount <= 0.f)
	{
		Health -= Amount;
		Die();
	}
	else
	{
		Health -= Amount;
	}
}
void AVampire::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}

void AVampire::Die()
{

}


void AVampire::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}
void AVampire::LMBDown()
{
	bLMBDown = true;
	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}

	}
	else if(EquippedWeapon)
	{
		Attack();
	}

}

void AVampire::LMBUp()
{

	bLMBDown = false;
}

void AVampire::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}

	EquippedWeapon = WeaponToSet;
}

void AVampire::Attack()
{
	bAttacking = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.35f);
		AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
	}
}
