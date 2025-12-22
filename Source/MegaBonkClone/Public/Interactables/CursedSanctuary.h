// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/SanctuaryBase.h"
#include "CursedSanctuary.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API ACursedSanctuary : public ASanctuaryBase
{
	GENERATED_BODY()
protected:
	virtual void ApplyEffect_Implementation(AActor* Player)override;
};
