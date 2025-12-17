// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PickupItem/PickupBaseActor.h"
#include "ResourcePickup.generated.h"

UENUM(BlueprintType)
enum class EResourceType : uint8 { Gold, Exp, Health};
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API AResourcePickup : public APickupBaseActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EResourceType ResourceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount = 10.0f;

	virtual void OnPickupComplete_Implementation() override;

};
