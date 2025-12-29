// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeShopWidget.generated.h"

class URewardSystemComponent;
class UUpgradeSlotWidget;
class UButton;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UUpgradeShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void InitWithRewards(URewardSystemComponent* InRewardComp, const TArray<FRewardOption>& InOptions);

private:
	UFUNCTION()
	void OnSlotPicked(int32 SlotIndex);

	UFUNCTION()
	void OnExitClicked();

private:
	UPROPERTY()
	TObjectPtr<URewardSystemComponent> RewardComp = nullptr;

	UPROPERTY()
	TArray<FRewardOption> CachedOptions;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUpgradeSlotWidget> Slot1 = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUpgradeSlotWidget> Slot2 = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUpgradeSlotWidget> Slot3 = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton = nullptr;

};
