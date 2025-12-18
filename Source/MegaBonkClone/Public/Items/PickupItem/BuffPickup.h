// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PickupItem/PickupBaseActor.h"
#include "Data/TypeEnums.h"
#include "BuffPickup.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API ABuffPickup : public APickupBaseActor
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
    EBuffType BuffType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
    float BuffDuration = 5.0f;

    virtual void OnPickupComplete_Implementation() override;
};
