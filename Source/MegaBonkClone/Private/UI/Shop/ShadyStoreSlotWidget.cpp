// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShadyStoreSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Interactables/ShadyGuyActor.h" // FShopSlotInfo

void UShadyStoreSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotButton)
	{
		SlotButton->OnClicked.RemoveDynamic(this, &UShadyStoreSlotWidget::HandleClicked);
		SlotButton->OnClicked.AddDynamic(this, &UShadyStoreSlotWidget::HandleClicked);
	}
}

void UShadyStoreSlotWidget::SetSlotInfo(const FShopSlotInfo& Info, int32 InSlotIndex)
{
	CachedIndex = InSlotIndex;

	// Title/Desc
	if (Title)       Title->SetText(Info.ItemName);
	if (Description) Description->SetText(Info.ItemDescription);

	// Image
	if (Image)
	{
		Image->SetBrushFromTexture(Info.IconTexture, true);
	}

	// Type(원하는 문구로 바꿔도 됨)
	if (Type)
	{
		// 등급 표시 or 가격 표시
		// Type->SetText(FText::AsNumber(Info.Price));
		Type->SetText(UEnum::GetDisplayValueAsText(Info.ItemGrade));
	}
}

void UShadyStoreSlotWidget::HandleClicked()
{
	if (CachedIndex == INDEX_NONE) return;
	OnSlotClicked.Broadcast(CachedIndex);
}
