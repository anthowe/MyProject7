// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "Vampire.h"




APickup::APickup()
{
	CoinCount = 1;
}

void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT("Pickup::On Overlap Begin"));

	if (OtherActor)
	{
		AVampire* MyCharacter = Cast<AVampire>(OtherActor);

		if (MyCharacter)
		{
			MyCharacter->IncrementCoins(CoinCount);
			Destroy();
		}
	}
}


void APickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	UE_LOG(LogTemp, Warning, TEXT("Pickup::On Overlap End"));
}
