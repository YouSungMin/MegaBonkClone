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

	//if (StatType) 
	//{
	//StatType->SetText(FText::FromString(
	//	StaticEnum<EItemGrade>()->GetNameStringByValue((int64)Info.Rarity)));
	//}

	if (StatType)
	{
		const FString GradeText =
			(Info.Rarity == EItemGrade::None)
			? TEXT("New")
			: StaticEnum<EItemGrade>()->GetNameStringByValue((int64)Info.Rarity);

		StatType->SetText(FText::FromString(GradeText));
	}

	//if (Level) Level->SetText(FText::AsNumber(Info.NewLevel));
	if (Level)
	{
		const FText LevelText =
			(Info.Rarity == EItemGrade::None)
			? FText::FromString(TEXT("New"))
			: FText::FromString(FString::Printf(TEXT("LV %d"), Info.NewLevel));

		Level->SetText(LevelText);
	}

	const UEnum* StatEnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/MegaBonkClone.EItemStatType"), true);

	if (StatEnumPtr)
	{
		// 2. Enum 값을 int64로 캐스팅하여 DisplayName을 가져옵니다.
		FText DisplayName = StatEnumPtr->GetDisplayNameTextByValue((int64)Info.StatType);

		// 3. 텍스트 설정
		if (Type)
		{
			Type->SetText(DisplayName);
		}
	}
	//if (Type) Type->SetText(UEnum::GetDisplayValueAsText(Info.StatType));
	if (Title) Title->SetText(Info.Name);
	if (Current) Current ->SetText(FText::AsNumber(Info.CurrentStatValue));
	if (Next) Next->SetText(FText::AsNumber(Info.NextStatValue));
}



void UUpgradeSlotWidget::HandleClicked()
{
	if (CachedIndex == INDEX_NONE) return;
	OnSlotClicked.Broadcast(CachedIndex);
}

