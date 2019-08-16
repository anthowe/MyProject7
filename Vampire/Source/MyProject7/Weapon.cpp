// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Vampire.h"


AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent > (TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());

	WeaponState = EWeaponState::EWS_Pickup;
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if ((WeaponState == EWeaponState::EWS_Pickup)  &&OtherActor)
	{
		AVampire* Vampire = Cast<AVampire>(OtherActor);
		if (Vampire)
		{
			Vampire->SetActiveOverlappingItem(this);
		}
	}
}
void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor)
	{
		AVampire* Vampire = Cast<AVampire>(OtherActor);
		if (Vampire)
		{
			Vampire->SetActiveOverlappingItem(nullptr);
		}
	}
}

void AWeapon::Equip(AVampire* Char)
{
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	WeaponMesh->SetSimulatePhysics(false);

	const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket");
	if (RightHandSocket)
	{
		RightHandSocket->AttachActor(this, Char->GetMesh());
		bRotate = false;
		Char->SetEquippedWeapon(this);
	}
	if (OnEquipSound)UGameplayStatics::PlaySound2D(this, OnEquipSound);
}