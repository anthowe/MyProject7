// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Vampire.h"



void UMyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
	if (Pawn)
	{
		MyCharacter = Cast<AVampire>(Pawn);
	}

}


void UMyAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.0f);
		MovementSpeed = LateralSpeed.Size();

		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		if (MyCharacter == nullptr)
		{
			MyCharacter = Cast<AVampire>(Pawn);
		}
	}

}