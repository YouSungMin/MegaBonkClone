// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopDetailWidget.generated.h"



/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UTopDetailWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void RefreshTimerText(float InMin,float InSec);

	UFUNCTION()
	void RefreshGoldCoinText(float InGold);

	UFUNCTION()
	void RefreshSilverCoinText(float InSilver);

	UFUNCTION()
	void RefreshKillCountText(float InKillCount);

	UFUNCTION()
	void RefreshGameTimeText(float InMin, float InSec);

	UFUNCTION()
	void RefreshLevelText(float InLevel);

	UFUNCTION()
	void RefreshExpProgressbar(float InCurrentExp, float InMaxExp);

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void InitializeTopDetail(class UStatusComponent* StatusComp);

	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Top|Detail", meta = (BindWidget))
	TObjectPtr<class UTextBlock> TimerMinText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Top|Detail", meta = (BindWidget))
	TObjectPtr<class UTextBlock> TimerSecText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Top|Detail", meta = (BindWidget))
	TObjectPtr<class UTextBlock> SilverCoinText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Top|Detail", meta = (BindWidget))
	TObjectPtr<class UTextBlock> GoldCoinText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Top|Detail", meta = (BindWidget))
	TObjectPtr<class UTextBlock> KillCountText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Top|Detail", meta = (BindWidget))
	TObjectPtr<class UTextBlock> GameTimeMinText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Top|Detail", meta = (BindWidget))
	TObjectPtr<class UTextBlock> GameTimeSecText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Top|Detail", meta = (BindWidget))
	TObjectPtr<class UTextBlock> LevelText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Top|Detail", meta = (BindWidget))
	TObjectPtr<class UProgressBar> ExpProgressbar = nullptr;
	

};
