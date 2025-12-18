// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/SanctuaryBase.h"
#include "MagneticSanctuary.generated.h"

UCLASS()
class MEGABONKCLONE_API AMagneticSanctuary : public ASanctuaryBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagneticSanctuary();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ApplyEffect_Implementation(AActor* Player)override;
};
