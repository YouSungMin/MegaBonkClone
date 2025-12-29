// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/UpgradeSlotWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/RewardSystemComponent.h"

void UUpgradeSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotButton)
	{
		SlotButton->OnClicked.RemoveDynamic(this, &UUpgradeSlotWidget::HandleClicked);
		SlotButton->OnClicked.AddDynamic(this, &UUpgradeSlotWidget::HandleClicked);
	}
}

void UUpgradeSlotWidget::SetSlotInfo(const FRewardOption& Info, int32 InSlotIndex)
{
	CachedIndex = InSlotIndex;

	if (Image)
	{
		UTexture2D* Tex = Info.Icon.LoadSynchronous();
		Image->SetBrushFromTexture(Tex, true);
	}

	StatType->SetText(UEnum::GetDisplayValueAsText(Info.Rarity));
	Type->SetText(UEnum::GetDisplayValueAsText(Info.Type));
	//if (Description) Description->SetText(Info.Description);
	if (Level) Level->SetText(FText::AsNumber(Info.NewLevel));
	if (Title) Title->SetText(Info.Name);
	if (Current) Current ->SetText(FText::AsNumber(Info.CurrentStatValue));
	if (Next) Next->SetText(FText::AsNumber(Info.NextStatValue));
}



void UUpgradeSlotWidget::HandleClicked()
{
	if (CachedIndex == INDEX_NONE) return;
	OnSlotClicked.Broadcast(CachedIndex);
}

