// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MicroShopWidget.generated.h"

//class FMicrowaveSlotInfo;
class AMicrowaveActor;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UMicroShopWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable)
	void InitWithMicrowave(AMicrowaveActor* InMicrowave, const TArray<struct FMicrowaveSlotInfo>& InList);

private:
	UPROPERTY()
	TObjectPtr<AMicrowaveActor> MicrowaveRef;

	UPROPERTY()
	TArray<FMicrowaveSlotInfo> CachedList;
};
