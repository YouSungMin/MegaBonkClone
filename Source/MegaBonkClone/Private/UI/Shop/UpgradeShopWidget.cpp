// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/UpgradeShopWidget.h"
#include "UI/Shop/UpgradeSlotWidget.h"
#include "Framework/MainHUD.h"
#include "Components/Button.h"
#include "Components/RewardSystemComponent.h"

void UUpgradeShopWidget::NativeConstruct()
{
	if (ExitButton)
	{
		ExitButton->OnClicked.RemoveDynamic(this, &UUpgradeShopWidget::OnExitClicked);
		ExitButton->OnClicked.AddDynamic(this, &UUpgradeShopWidget::OnExitClicked);
	}
	// 슬롯 클릭 이벤트 연결
	if (Slot1)
	{
		Slot1->OnSlotClicked.RemoveDynamic(this, &UUpgradeShopWidget::OnSlotPicked);
		Slot1->OnSlotClicked.AddDynamic(this, &UUpgradeShopWidget::OnSlotPicked);
	}
	if (Slot2)
	{
		Slot2->OnSlotClicked.RemoveDynamic(this, &UUpgradeShopWidget::OnSlotPicked);
		Slot2->OnSlotClicked.AddDynamic(this, &UUpgradeShopWidget::OnSlotPicked);
	}
	if (Slot3)
	{
		Slot3->OnSlotClicked.RemoveDynamic(this, &UUpgradeShopWidget::OnSlotPicked);
		Slot3->OnSlotClicked.AddDynamic(this, &UUpgradeShopWidget::OnSlotPicked);
	}
}

void UUpgradeShopWidget::InitWithRewards(URewardSystemComponent* InRewardComp, const TArray<FRewardOption>& InOptions)
{
	RewardComp = InRewardComp;
	CachedOptions = InOptions;

	// 3개 슬롯에 데이터 주입
	Slot1->SetSlotInfo(CachedOptions[0], 0);
	Slot2->SetSlotInfo(CachedOptions[1], 1);
	Slot3->SetSlotInfo(CachedOptions[2], 2);
}


void UUpgradeShopWidget::OnSlotPicked(int32 SlotIndex)
{
	if (!RewardComp) return;
	if (!CachedOptions.IsValidIndex(SlotIndex)) return;

	UE_LOG(LogTemp, Warning, TEXT("[UpgradeShop] Picked SlotIndex=%d"), SlotIndex);

	RewardComp->SelectReward(CachedOptions[SlotIndex]);

	// UI 닫기 + 게임 재개(혹시 여기서도 안전하게)
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
		{
			HUD->CloseCenterUI();
		}
		PC->SetPause(false);
	}
}

void UUpgradeShopWidget::OnExitClicked()
{
	// 보상 강제 선택이면 ExitButton 숨기는 게 보통이라, 일단은 닫기만.
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
		{
			HUD->CloseCenterUI();
		}
		PC->SetPause(false);
	}
}
