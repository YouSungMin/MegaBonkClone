// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/RechargeShopWidget.h"
#include "UI/Shop/RechargeSlotWidget.h"

void URechargeShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Slot1->ClearRechargeSlot();
	Slot2->ClearRechargeSlot();
	Slot3->ClearRechargeSlot();
}

void URechargeShopWidget::InitWithSanctuary(ARechargeSanctuary* InSanctuary)
{
	SanctuaryRef = InSanctuary;
	if (!SanctuaryRef) return;

	SanctuaryRef->OnRewardsGenerated.RemoveDynamic(this, &URechargeShopWidget::HandleRewardsGenerated);
	SanctuaryRef->OnRewardsGenerated.AddDynamic(this, &URechargeShopWidget::HandleRewardsGenerated);
}

void URechargeShopWidget::HandleRewardsGenerated(const TArray<FSanctuaryRewardInfo>& Rewards, ARechargeSanctuary* SanctuaryActor)
{
	Slot1->ClearRechargeSlot();
	Slot2->ClearRechargeSlot();
	Slot3->ClearRechargeSlot();

	Slot1->SetRewardInfo(Rewards[0]);
	Slot2->SetRewardInfo(Rewards[1]);
	Slot3->SetRewardInfo(Rewards[2]);
}

void URechargeShopWidget::ShuffleRewards(TArray<FSanctuaryRewardInfo>& InOutRewards)
{
}
