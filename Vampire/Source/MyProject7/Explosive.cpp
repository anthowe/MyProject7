// Fill out your copyright notice in the Description page of Project Settings.

#include "Explosive.h"
#include "Vampire.h"

AExplosive::AExplosive()
{
	Damage = 15.f;
	

	
}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT("Explosive::On Overlap Begin"));

	if (OtherActor)
	{
		AVampire* MyCharacter = Cast<AVampire>(OtherActor);

		if (MyCharacter)
		{
			
			Destroy();
		}
	}
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	UE_LOG(LogTemp, Warning, TEXT("Explosive::On Overlap End"));
}

