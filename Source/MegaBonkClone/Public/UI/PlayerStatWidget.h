// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UPlayerStatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void RefreshStat(const FText& InName, float InValue);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (BindWidget))
	TWeakObjectPtr<class UTextBlock> StatName;	//Ω∫≈» ¿Ã∏ß

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (BindWidget))
	TWeakObjectPtr<class UTextBlock> StatValue; //Ω∫≈» ∞™
};
