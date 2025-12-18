// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickupItem/BuffPickup.h"
#include "Characters/Components/StatusComponent.h"

void ABuffPickup::OnPickupComplete_Implementation()
{
	UStatusComponent* statusComponent = PickupOwner->FindComponentByClass<UStatusComponent>();

	if(statusComponent)
	{
		statusComponent->ApplyBuff(BuffType, BuffDuration);
	}
	Super::OnPickupComplete_Implementation();
}
