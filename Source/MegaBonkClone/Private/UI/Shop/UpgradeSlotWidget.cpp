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

	if (Type)
	{
		// 빌드 버전에서도 안전하게 텍스트를 가져옵니다.
		FText DisplayName = GetStatDisplayName(Info.StatType);
		Type->SetText(DisplayName);
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

FText UUpgradeSlotWidget::GetStatDisplayName(EItemStatType InStatType) const
{
	switch (InStatType)
	{
	case EItemStatType::MaxHP:
		return FText::FromString(TEXT("최대 체력"));
	case EItemStatType::HPRegen:
		return FText::FromString(TEXT("체력 재생"));
	case EItemStatType::OverHeal:
		return FText::FromString(TEXT("오버 힐"));
	case EItemStatType::Shield:
		return FText::FromString(TEXT("실드"));
	case EItemStatType::Armor:
		return FText::FromString(TEXT("방어력"));
	case EItemStatType::EvasionChance:
		return FText::FromString(TEXT("회피율"));
	case EItemStatType::LifeDrain:
		return FText::FromString(TEXT("생명 흡수"));
	case EItemStatType::Thorn:
		return FText::FromString(TEXT("가시"));
	case EItemStatType::ExpGain:
		return FText::FromString(TEXT("경험치 획득량"));
	case EItemStatType::SilverGain:
		return FText::FromString(TEXT("실버 획득량"));
	case EItemStatType::GoldGain:
		return FText::FromString(TEXT("골드 획득량"));
	case EItemStatType::PickUpRange:
		return FText::FromString(TEXT("픽업 범위"));
	case EItemStatType::Difficulty:
		return FText::FromString(TEXT("난이도"));
	case EItemStatType::Damage:
		return FText::FromString(TEXT("데미지"));
	case EItemStatType::CritChance:
		return FText::FromString(TEXT("치명타 확률"));
	//case EItemStatType::MegaCritChance:
	//	return FText::FromString(TEXT("메가 치명타 확률"));
	case EItemStatType::AttackSpeed:
		return FText::FromString(TEXT("공격속도"));
	case EItemStatType::ProjectileCount:
		return FText::FromString(TEXT("발사체 수"));
	case EItemStatType::ProjectileReflectCount:
		return FText::FromString(TEXT("발사체 반사"));
	case EItemStatType::AttackSize:
		return FText::FromString(TEXT("발사체 크기"));
	case EItemStatType::ProjectileSpeed:
		return FText::FromString(TEXT("발사체 속도"));
	case EItemStatType::AttackDuration:
		return FText::FromString(TEXT("지속 시간"));
	case EItemStatType::KnockBack:
		return FText::FromString(TEXT("넉백"));
	case EItemStatType::MoveSpeed:
		return FText::FromString(TEXT("이동 속도"));
	case EItemStatType::JumpPower:
		return FText::FromString(TEXT("점프 높이"));
	case EItemStatType::ExtraJump:
		return FText::FromString(TEXT("점프 횟수"));
	case EItemStatType::Luck:
		return FText::FromString(TEXT("행운"));
	default:
		return FText::FromString(TEXT("알 수 없음"));
	}
}

