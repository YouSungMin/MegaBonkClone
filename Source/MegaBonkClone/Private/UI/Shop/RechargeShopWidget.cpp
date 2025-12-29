// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/RechargeShopWidget.h"
#include "UI/Shop/RechargeSlotWidget.h"
#include "Components/Button.h"
#include "Framework/MainHUD.h"

void URechargeShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Slot1->ClearRechargeSlot();
	Slot2->ClearRechargeSlot();
	Slot3->ClearRechargeSlot();

	if (Slot1)
	{
		Slot1->OnSlotClicked.RemoveDynamic(this, &URechargeShopWidget::OnAnySlotClicked);
		Slot1->OnSlotClicked.AddUniqueDynamic(this, &URechargeShopWidget::OnAnySlotClicked);
	}
	if (Slot2)
	{
		Slot2->OnSlotClicked.RemoveDynamic(this, &URechargeShopWidget::OnAnySlotClicked);
		Slot2->OnSlotClicked.AddUniqueDynamic(this, &URechargeShopWidget::OnAnySlotClicked);
	}
	if (Slot3)
	{
		Slot3->OnSlotClicked.RemoveDynamic(this, &URechargeShopWidget::OnAnySlotClicked);
		Slot3->OnSlotClicked.AddUniqueDynamic(this, &URechargeShopWidget::OnAnySlotClicked);
	}
	// Exit 버튼
	if (ExitButton)
	{
		ExitButton->OnClicked.AddUniqueDynamic(this, &URechargeShopWidget::OnExitClicked);
		UE_LOG(LogTemp, Warning, TEXT("[RechargeShop] ExitButton bind OK"));

	}
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

void URechargeShopWidget::OnAnySlotClicked(const FSanctuaryRewardInfo& RewardInfo)
{
	if (!SanctuaryRef) return;

	const FString StatName = UEnum::GetDisplayValueAsText(RewardInfo.StatType).ToString();
	const FString GradeName = UEnum::GetDisplayValueAsText(RewardInfo.Rarity).ToString();

	//선택한슬롯확인
	UE_LOG(LogTemp, Warning, TEXT("[RechargeShop] Reward Clicked => Stat=%s / Grade=%s / Value=%.2f"), *StatName, *GradeName, RewardInfo.Value);

	SanctuaryRef->ApplySelectedReward(RewardInfo);

	UE_LOG(LogTemp, Warning, TEXT("슬롯선택 Clicked!!"));

	// UI 닫기 
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
		{
			HUD->CloseCenterUI();
		}
	}
}

void URechargeShopWidget::OnExitClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Exit Clicked!!"));
	// 그냥 닫기만
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
		{
			HUD->CloseCenterUI();
		}
	}
}
