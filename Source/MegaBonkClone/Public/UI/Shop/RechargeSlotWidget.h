// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interactables/RechargeSanctuary.h"
#include "RechargeSlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRechargeSlotClicked, const FSanctuaryRewardInfo&, RewardInfo);

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API URechargeSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;  

	UFUNCTION(BlueprintCallable, Category = "RechargeSlot")
	void SetRewardInfo(const FSanctuaryRewardInfo& RewardInfo);

	UFUNCTION(BlueprintCallable, Category = "RechargeSlot")
	void ClearRechargeSlot();

	//눌렀을떄 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnRechargeSlotClicked OnSlotClicked;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonSlot = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RechargeSlot", meta = (BindWidget))
	TObjectPtr<UTextBlock> Type = nullptr; //타입

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RechargeSlot", meta = (BindWidget))
	TObjectPtr<UTextBlock> Value = nullptr;	//값

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RechargeSlot", meta = (BindWidget))
	TObjectPtr<UTextBlock> Description = nullptr;	//부가설명

private:
	UFUNCTION()
	void HandleSlotClicked();

	FSanctuaryRewardInfo CachedReward;
	bool bHasReward = false;

};
