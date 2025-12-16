// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickupItem/SpecialActionPickup.h"

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
}
