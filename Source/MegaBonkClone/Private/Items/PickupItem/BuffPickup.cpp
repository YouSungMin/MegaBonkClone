// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickupItem/BuffPickup.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"

void ABuffPickup::OnPickupComplete_Implementation()
{
	if (auto* Player = Cast<APlayerCharacter>(PickupOwner))
	{
		//Player->ApplyBuff(BuffType, Duration);
	}
	Super::OnPickupComplete_Implementation();
}
