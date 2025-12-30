// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShadyStoreSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Interactables/ShadyGuyActor.h" // FShopSlotInfo
#include "Characters/Components/StatusComponent.h"

void UShadyStoreSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotButton)
	{
		SlotButton->OnClicked.RemoveDynamic(this, &UShadyStoreSlotWidget::HandleClicked);
		SlotButton->OnClicked.AddDynamic(this, &UShadyStoreSlotWidget::HandleClicked);
	}

	// 플레이어에서 StatusComponent 찾아 캐싱
	if (!CachedStatus)
	{
		if (APawn* Pawn = GetOwningPlayerPawn())
		{
			CachedStatus = Pawn->FindComponentByClass<UStatusComponent>();
		}
	}

	// 골드 변경 시마다 UI 갱신
	if (CachedStatus)
	{
		CachedStatus->OnGoldChanged.RemoveDynamic(this, &UShadyStoreSlotWidget::HandleGoldChanged);
		CachedStatus->OnGoldChanged.AddDynamic(this, &UShadyStoreSlotWidget::HandleGoldChanged);
	}
}

void UShadyStoreSlotWidget::SetSlotInfo(const FShopSlotInfo& Info, int32 InSlotIndex)
{
	CachedIndex = InSlotIndex;
	CachedPrice = (float)Info.Price; // 비교용 숫자로 저장

	// Title/Desc
	if (Title)       Title->SetText(Info.ItemName);
	if (Description) Description->SetText(Info.ItemDescription);

	// Image
	if (Image)		Image->SetBrushFromTexture(Info.IconTexture, true);

	// Type(원하는 문구로 바꿔도 됨)
	if (Type)
	{
		// 등급 표시 or 가격 표시
	Type->SetText(FText::FromString(
		StaticEnum<EItemGrade>()->GetNameStringByValue((int64)Info.ItemGrade)));
	}

	if (GoldText)	GoldText->SetText(FText::AsNumber(Info.Price));

	RefreshAffordableUI(); // 여기서 버튼/색 처리
}

void UShadyStoreSlotWidget::HandleClicked()
{
	if (CachedIndex == INDEX_NONE) return;
	OnSlotClicked.Broadcast(CachedIndex);
}

void UShadyStoreSlotWidget::HandleGoldChanged(float NewGold)
{
	RefreshAffordableUI();
}

void UShadyStoreSlotWidget::RefreshAffordableUI()
{
	float CurrentGoldValue = CachedStatus ? CachedStatus->GetCurrentGold() : 0.f;

	bAffordable = (CurrentGoldValue >= CachedPrice);

	// 1) 클릭 불가
	if (SlotButton)
	{
		SlotButton->SetIsEnabled(bAffordable);
	}

	// 2) 어둡게 (전체 위젯 투명도)
	SetRenderOpacity(bAffordable ? 1.0f : 0.45f);
}
