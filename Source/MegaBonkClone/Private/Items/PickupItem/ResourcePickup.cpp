// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickupItem/ResourcePickup.h"
#include "Characters/Components/StatusComponent.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"

void AResourcePickup::OnPickupComplete_Implementation()
{
	if (auto* Player = Cast<APlayerCharacter>(PickupOwner))
	{
		if (UStatusComponent* targetStatus = Player->GetStatusComponent()) {
			switch (ResourceType)
			{
			case EResourceType::Gold:
				UE_LOG(LogTemp, Log, TEXT("골드 추가"));
				targetStatus->AddGold(Amount);
				break;
			case EResourceType::Exp:
				UE_LOG(LogTemp, Log, TEXT("경험치 추가"));
				targetStatus->AddExp(Amount);
				break;
			case EResourceType::Health:
				UE_LOG(LogTemp, Log, TEXT("HP 추가"));
				targetStatus->AddCurrentHP(Amount);
				break;
			default:
				break;
			}
		}
		
	}
	Super::OnPickupComplete_Implementation();
}
