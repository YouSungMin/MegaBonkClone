// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShadyStoreShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UShadyStoreShopWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	void InitWithShadyGuy(class AShadyGuyActor* InGuy, const TArray<struct FShopSlotInfo>& Items);

	
};
