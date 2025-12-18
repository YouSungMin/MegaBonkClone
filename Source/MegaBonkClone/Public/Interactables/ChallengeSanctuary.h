
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/SanctuaryBase.h"
#include "ChallengeSanctuary.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API AChallengeSanctuary : public ASanctuaryBase
{
	GENERATED_BODY()
public:
	virtual void ApplyEffect_Implementation(AActor* Player)override;
};
