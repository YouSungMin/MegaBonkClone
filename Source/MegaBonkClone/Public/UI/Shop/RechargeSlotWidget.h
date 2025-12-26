// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interactables/RechargeSanctuary.h"
#include "RechargeSlotWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API URechargeSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "RechargeSlot")
	void SetRewardInfo(const FSanctuaryRewardInfo& RewardInfo);

	UFUNCTION(BlueprintCallable, Category = "RechargeSlot")
	void ClearRechargeSlot();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RechargeSlot", meta = (BindWidget))
	TObjectPtr<UTextBlock> Type = nullptr; //타입

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RechargeSlot", meta = (BindWidget))
	TObjectPtr<UTextBlock> Value = nullptr;	//값

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RechargeSlot", meta = (BindWidget))
	TObjectPtr<UTextBlock> Description = nullptr;	//부가설명

};
