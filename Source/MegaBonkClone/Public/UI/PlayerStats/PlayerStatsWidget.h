// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerStatsCloseRequested);
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UPlayerStatsWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintAssignable, Category = "UI|Inventory")
	FOnPlayerStatsCloseRequested OnPlayerStatsCloseRequested;
};
