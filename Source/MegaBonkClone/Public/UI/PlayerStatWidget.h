// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UPlayerStatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetRowStat(const FText& InLabel, const FText& InValue);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (BindWidget))
	TWeakObjectPtr<UTextBlock> StatLabel;	//스탯 이름

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (BindWidget))
	TWeakObjectPtr<UTextBlock> StatValue; //스탯 값
};
