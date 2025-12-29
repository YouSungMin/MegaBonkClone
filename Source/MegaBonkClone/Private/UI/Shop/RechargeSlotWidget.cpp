// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/RechargeSlotWidget.h"
#include "Components/TextBlock.h"	//TextBlock
#include "Components/Button.h"
//#include "RechargeShopWidget.h"

void URechargeSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ButtonSlot)
	{
		ButtonSlot->OnClicked.AddDynamic(this, &URechargeSlotWidget::HandleSlotClicked);
	}
}

void URechargeSlotWidget::SetRewardInfo(const FSanctuaryRewardInfo& RewardInfo)
{
	CachedReward = RewardInfo;
	bHasReward = true;

	//Type->SetText(UEnum::GetDisplayValueAsText(RewardInfo.Rarity));
	Type->SetText(FText::FromString(
		StaticEnum<EItemGrade>()->GetNameStringByValue((int64)RewardInfo.Rarity)));
	Value->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), RewardInfo.Value)));
	StateType->SetText(UEnum::GetDisplayValueAsText(RewardInfo.StatType));

}

void URechargeSlotWidget::ClearRechargeSlot()
{
	bHasReward = false;

	Type->SetText(FText::GetEmpty());
	Value->SetText(FText::GetEmpty());
	StateType->SetText(FText::GetEmpty());
}

void URechargeSlotWidget::HandleSlotClicked()
{
	if (!bHasReward) return;
	OnSlotClicked.Broadcast(CachedReward);
}

