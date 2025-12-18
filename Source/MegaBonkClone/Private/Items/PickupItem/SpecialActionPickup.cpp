// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickupItem/SpecialActionPickup.h"
#include "Interfaces/PickupInterface.h"
#include "Items/PickupItem/ResourcePickup.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>

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
	UE_LOG(LogTemp, Log, TEXT("모든 적 제거 효과"));
}

void ASpecialActionPickup::ExecuteMagnet()
{
	if (auto* player = Cast<APlayerCharacter>(PickupOwner.Get()))
	{
		player->ActivateMagnetEffect();
	}
}
