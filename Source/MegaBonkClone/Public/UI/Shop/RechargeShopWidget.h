// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interactables/RechargeSanctuary.h"
#include "RechargeShopWidget.generated.h"

class URechargeSlotWidget;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API URechargeShopWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "RechargeShop")
	void InitWithSanctuary(ARechargeSanctuary* InSanctuary);

private:
	UFUNCTION()
	void HandleRewardsGenerated(const TArray<FSanctuaryRewardInfo>& Rewards, ARechargeSanctuary* SanctuaryActor);

	void ShuffleRewards(TArray<FSanctuaryRewardInfo>& InOutRewards);

private:
	UPROPERTY()
	TObjectPtr<ARechargeSanctuary> SanctuaryRef = nullptr;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URechargeSlotWidget> Slot1 = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URechargeSlotWidget> Slot2 = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URechargeSlotWidget> Slot3 = nullptr;
};
