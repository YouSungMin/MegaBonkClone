// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/SanctuaryBase.h"
#include "GreedSanctuary.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API AGreedSanctuary : public ASanctuaryBase
{
	GENERATED_BODY()
public:
	virtual void ApplyEffect_Implementation(AActor* Player)override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty")
	float DifficultyAmount = 0.05f;
};
