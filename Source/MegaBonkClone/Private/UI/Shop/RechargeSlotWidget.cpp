// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/RechargeSlotWidget.h"
#include "Components/TextBlock.h"	//TextBlock

void URechargeSlotWidget::SetRewardInfo(const FSanctuaryRewardInfo& RewardInfo)
{
	Type->SetText(UEnum::GetDisplayValueAsText(RewardInfo.StatType));
	Value->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), RewardInfo.Value)));
	Description->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), RewardInfo.Rarity)));
}

void URechargeSlotWidget::ClearRechargeSlot()
{
	Type->SetText(FText::GetEmpty());
	Value->SetText(FText::GetEmpty());
	Description->SetText(FText::GetEmpty());
}
