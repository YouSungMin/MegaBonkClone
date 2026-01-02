// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/GreedSanctuary.h"
#include "Characters/Components/StatusComponent.h"

void AGreedSanctuary::ApplyEffect_Implementation(AActor* Player)
{
	UStatusComponent* statusComponent = Player->FindComponentByClass<UStatusComponent>();

	if (statusComponent)
	{
		statusComponent->AddShrineDifficulty(DifficultyAmount);
		UE_LOG(LogTemp,Log,TEXT("Difficulty = %f"), statusComponent->GetResultDifficulty());
		bIsUsed = true;
		SanNotifyToHUD(FString::Printf(TEXT("탐욕의 제단 활성화됨!")));
		Destroy();
	}
}
