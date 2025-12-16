// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickupItem/SpecialActionPickup.h"
#include "Interfaces/PickupInterface.h"
#include "Items/PickupItem/ResourcePickup.h"

void ASpecialActionPickup::OnPickupComplete_Implementation()
{
	switch (ActionType)
	{
	case ESpecialActionType::Nuke:
		ExecuteNuke();
		break;
	case ESpecialActionType::Magnet:
		ExecuteMagnet();
		break;
	default:
		break;
	}
	Super::OnPickupComplete_Implementation();
}

void ASpecialActionPickup::ExecuteNuke()
{
	
}

void ASpecialActionPickup::ExecuteMagnet()
{
	UE_LOG(LogTemp,Log, TEXT("자석 효과"));

	TArray<AActor*> FoundItems;
}
