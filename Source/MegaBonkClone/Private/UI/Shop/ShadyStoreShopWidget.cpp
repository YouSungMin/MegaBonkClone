// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShadyStoreShopWidget.h"
#include "UI/Shop/ShadyStoreSlotWidget.h"
#include "Components/Button.h"
#include "Framework/MainHUD.h"
#include "Interactables/ShadyGuyActor.h"

void UShadyStoreShopWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ExitButton)
    {
        ExitButton->OnClicked.RemoveDynamic(this, &UShadyStoreShopWidget::OnExitClicked);
        ExitButton->OnClicked.AddDynamic(this, &UShadyStoreShopWidget::OnExitClicked);
    }
}

void UShadyStoreShopWidget::InitWithShadyGuy(AShadyGuyActor* InGuy, const TArray<struct FShopSlotInfo>& Items)
{
	GuyRef = InGuy;
	CachedItems = Items;

	// 슬롯 배열처럼 다루기
	UShadyStoreSlotWidget* Slots[3] = { Slot1, Slot2, Slot3 };

	for (int32 i = 0; i < 3; ++i)
	{
		if (!Slots[i]) continue;

		Slots[i]->OnSlotClicked.RemoveDynamic(this, &UShadyStoreShopWidget::OnSlotPicked);
		Slots[i]->OnSlotClicked.AddDynamic(this, &UShadyStoreShopWidget::OnSlotPicked);

		if (CachedItems.IsValidIndex(i))
		{
			Slots[i]->SetVisibility(ESlateVisibility::Visible);
			Slots[i]->SetSlotInfo(CachedItems[i], i);
		}
		else
		{
			Slots[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UShadyStoreShopWidget::OnSlotPicked(int32 SlotIndex)
{
	if (!GuyRef) return;

	UE_LOG(LogTemp, Warning, TEXT("[ShadyShop] Picked SlotIndex=%d"), SlotIndex);

	// 구매 처리
	GuyRef->ProcessPurchase(SlotIndex);

	// UI 닫기
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
		{
			HUD->CloseCenterUI();
		}
	}
}

void UShadyStoreShopWidget::OnExitClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("[ShadyShop] Exit Clicked"));

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
		{
			HUD->CloseCenterUI();
		}
	}
}