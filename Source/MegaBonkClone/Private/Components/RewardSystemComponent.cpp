// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/RewardSystemComponent.h"
#include "Components/InventoryComponent.h"
#include "Characters/Components/StatusComponent.h"
#include "Characters/Components/WeaponSystemComponent.h"
#include "Data/WeaponDataStructs.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Framework/MainHUD.h"
#include "Kismet/KismetMathLibrary.h"

URewardSystemComponent::URewardSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 기본 확률 가중치 설정
	RarityWeights.Add(EItemGrade::Common, 50.0f);
	RarityWeights.Add(EItemGrade::Rare, 30.0f);
	RarityWeights.Add(EItemGrade::Epic, 20.0f);
	RarityWeights.Add(EItemGrade::Legendary, 10.0f);
}

void URewardSystemComponent::GenerateLevelUpRewards()
{
	if (!FindComponents())
	{
		UE_LOG(LogTemp, Error, TEXT("RewardSystem: 컴포넌트가 유효하지 않아 보상을 생성할 수 없습니다."));
		return;
	}

	if (!WeaponDataTable || !ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("RewardSystem: Components or DataTables are missing!"));
		return;
	}

	// 2. 슬롯 여유 확인 (3개 제한)
	bool bCanNewWeapon = (WeaponComp->GetCurrentWeaponCount() < 3);
	bool bCanNewBook = (InventoryComp->GetSecretBookSlots().Num() < 3);

	TArray<FRewardOption> Pool;

	// =========================================================
	// [A] 무기 후보군 (무한 레벨업)
	// =========================================================
	for (FName RowName : WeaponDataTable->GetRowNames())
	{
		FWeaponData* Row = WeaponDataTable->FindRow<FWeaponData>(RowName, TEXT("Reward_Wep"));
		if (!Row) continue;

		bool bHas = WeaponComp->HasWeapon(RowName);

		// [조건] 보유 중이면 무조건 후보(강화), 미보유면 슬롯이 남아야 후보(신규)
		if (bHas || bCanNewWeapon)
		{
			FRewardOption Opt;
			Opt.ItemID = RowName;
			Opt.Type = EItemType::Weapon;
			Opt.Icon = Row->Icon;
			Opt.Name = Row->Name;
			Row->UpgradeOptions;
			int32 CurLv = WeaponComp->GetWeaponLevel(RowName);
			Opt.NewLevel = CurLv + 1;

			ApplyWeaponUpgradeLogic(Row, CurLv, bHas, Opt);
			Pool.Add(Opt);
		}
	}

	// =========================================================
	// [B] 비전서 후보군 (무한 레벨업)
	// =========================================================
	for (FName RowName : ItemDataTable->GetRowNames())
	{
		FItemData* Row = ItemDataTable->FindRow<FItemData>(RowName, TEXT("Reward_Book"));
		if (!Row) continue;

		// 비전서 타입만 필터링
		if (Row->ItemType != EItemType::SecretBook) continue;

		bool bHas = false;
		int32 CurLv = 0;
		for (const FInventorySlot& Slot : InventoryComp->GetSecretBookSlots())
		{
			if (Slot.ItemID == RowName) { bHas = true; CurLv = Slot.Level; break; }
		}

		// [조건] 보유 중이면 무조건 후보(강화), 미보유면 슬롯이 남아야 후보(신규)
		if (bHas || bCanNewBook)
		{
			FRewardOption Opt;
			Opt.ItemID = RowName;
			Opt.Type = EItemType::SecretBook;
			Opt.Icon = Row->Icon;
			Opt.Name = Row->Name;

			Opt.NewLevel = CurLv + 1;
			ApplySecretBookLogic(Row, CurLv, bHas, Opt);

			Pool.Add(Opt);
		}
	}

	// =========================================================
	// [C] 랜덤 3개 선정
	// =========================================================
	TArray<FRewardOption> FinalRewards;
	int32 PickNum = FMath::Min(Pool.Num(), 3);

	for (int32 i = 0; i < PickNum; ++i)
	{
		int32 RandIdx = FMath::RandRange(0, Pool.Num() - 1);
		FRewardOption Selected = Pool[RandIdx];

		// 등급 부여
		//Selected.Rarity = CalculateRandomRarity();

		UE_LOG(LogTemp, Log, TEXT("ItemID = %s, Type = %s, StateType = %s, NewLevel = %d, CurrentStatValue = %.1f, NextStatValue = %.1f, Rarity = %s"),
			*Selected.ItemID.ToString(),
			*UEnum::GetValueAsString(Selected.Type), 
			*UEnum::GetValueAsString(Selected.StatType),
			Selected.NewLevel,
			Selected.CurrentStatValue,
			Selected.NextStatValue,
			*UEnum::GetValueAsString(Selected.Rarity)
		);
		FinalRewards.Add(Selected);
		Pool.RemoveAtSwap(RandIdx); // 중복 방지
	}

	// UI 생성 요청
	if (OnRewardsGenerated.IsBound())
	{
		OnRewardsGenerated.Broadcast(FinalRewards);
	}

	if (APawn* PawnOwner = Cast<APawn>(GetOwner()))
	{
		if (APlayerController* PC = Cast<APlayerController>(PawnOwner->GetController()))
		{
			if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
			{
				HUD->ShowUpgrade(this, FinalRewards);
			}
		}
	}
	else if (APlayerController* PC = Cast<APlayerController>(GetOwner()))
	{
		if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
		{
			HUD->ShowUpgrade(this, FinalRewards);
		}
	}

	// 게임 일시정지
	if (APlayerController* PC = Cast<APlayerController>(GetOwner()))
	{
		PC->SetPause(true);
	}
}

void URewardSystemComponent::ApplyWeaponUpgradeLogic(const FWeaponData* Row, int32 CurLv, bool bHas, FRewardOption& OutOption)
{
	if (!Row) return;

	// 등급(Rarity) 결정 로직
	if (bHas)
	{
		// 강화라면: 확률에 따라 등급 결정 (Common ~ Legendary)
		OutOption.Rarity = CalculateRandomRarity();
	}
	else
	{
		// 신규 획득이라면: 등급 개념 없음 (기본 Common 설정)
		OutOption.Rarity = EItemGrade::None;
	}

	// 강화 옵션이 하나라도 있다면?
	if (Row->UpgradeOptions.Num() > 0)
	{
		// 랜덤 옵션 하나 선택
		int32 RandIndex = FMath::RandRange(0, Row->UpgradeOptions.Num() - 1);
		const FWeaponUpgradeOption& SelectedUpgrade = Row->UpgradeOptions[RandIndex];

		// 3. 배율 적용 (신규 획득이면 Common이라 1.0배, 강화면 등급에 따라 배율 적용)
		float Multiplier = 1.0f;

		// 신규가 아니고(bHas), 등급별 배율표에 값이 있다면 가져옴
		if (bHas && GradeMultipliers.Contains(OutOption.Rarity))
		{
			Multiplier = GradeMultipliers[OutOption.Rarity];
		}

		// 최종 증가량 계산
		float FinalValue = SelectedUpgrade.Value * Multiplier;

		// 결과 데이터 채우기
		OutOption.NextStatValue = FinalValue;
		// 3. 스탯 타입 매핑 및 기본값 가져오기
		float BaseValue = 0.0f;

		switch (SelectedUpgrade.StatType)
		{
		case EWeaponStatType::Damage:
			OutOption.StatType = EItemStatType::Damage;
			BaseValue = Row->WeaponDamage;
			break;

		case EWeaponStatType::ProjectileCount:
			OutOption.StatType = EItemStatType::ProjectileCount;
			BaseValue = Row->ProjectileCount;
			break;

		case EWeaponStatType::ProjectileSpeed:
			OutOption.StatType = EItemStatType::ProjectileSpeed;
			BaseValue = Row->ProjectileSpeed;
			break;

		case EWeaponStatType::ProjectileScale:
			OutOption.StatType = EItemStatType::AttackSize; // AttackSize로 매핑
			BaseValue = Row->ProjectileScale;
			break;

		case EWeaponStatType::ChainCount:
			OutOption.StatType = EItemStatType::AttackSize; // AttackSize로 매핑
			BaseValue = Row->ChainCount;
			break;

		case EWeaponStatType::CritChance:
			OutOption.StatType = EItemStatType::CritChance;
			BaseValue = Row->CriticalChance;
			break;

		case EWeaponStatType::CritDamage:
			OutOption.StatType = EItemStatType::CritChance;
			BaseValue = Row->CriticalDamage;
			break;

		case EWeaponStatType::KnockBack:
			OutOption.StatType = EItemStatType::KnockBack;
			BaseValue = Row->KnockBack;
			break;

		case EWeaponStatType::Cooldown:
			OutOption.StatType = EItemStatType::KnockBack;
			BaseValue = Row->Cooldown;
			break;

			// 나머지 케이스 처리...
		default:
			OutOption.StatType = EItemStatType::None;
			break;
		}

		// 현재 스탯 추정치 (단순 누적 방식 가정)
		OutOption.CurrentStatValue = BaseValue + (CurLv - 1 * SelectedUpgrade.Value); // *주의: 현재 스탯은 배율 적용 전 기준
		OutOption.NextStatValue = OutOption.CurrentStatValue + FinalValue;

		// 4. 설명 텍스트 생성
		if (bHas)
		{
			// 강화: "공격력 +10 (레어)" 형식
			FString StatName = UEnum::GetDisplayValueAsText(OutOption.StatType).ToString();
			OutOption.Description = FText::FromString(FString::Printf(TEXT("%s +%.1f"), *StatName, FinalValue));
		}
		else
		{
			// 신규: 무기 기본 설명 표시
			OutOption.Description = Row->Description;
		}
	}
	else
	{
		// 옵션 데이터가 없을 때 안전 처리
		OutOption.StatType = EItemStatType::None;
		OutOption.Description = bHas ? FText::FromString(TEXT("레벨 업!")) : Row->Description;
	}
}

void URewardSystemComponent::ApplySecretBookLogic(const FItemData* Row, int32 CurLv, bool bHas, FRewardOption& OutOption)
{
	if (!Row) return;

	// 등급(Rarity) 결정 로직
	if (bHas)
	{
		// 강화라면: 확률에 따라 등급 결정 (Common ~ Legendary)
		OutOption.Rarity = CalculateRandomRarity();
	}
	else
	{
		// 신규 획득이라면: 등급 개념 없음 (기본 Common 설정)
		OutOption.Rarity = EItemGrade::None;
	}

	if (Row->Modifiers.Num() > 0)
	{
		// 비전서는 0번 인덱스 효과를 메인으로 사용
		const FItemStatModifier& MainMod = Row->Modifiers[0];

		// 1. StatType 설정
		OutOption.StatType = MainMod.StatType;

		// 2. 수치 계산
		if (bHas)
		{
			// [강화] 현재 수치 = 기본값 + (현재레벨 - 1) * 중첩값
			OutOption.CurrentStatValue = MainMod.BaseValue + ((CurLv - 1) * MainMod.StackValue);

			// 다음 수치 = 현재 수치 + 중첩값
			OutOption.NextStatValue = OutOption.CurrentStatValue + MainMod.StackValue;

			// 설명 텍스트 생성
			FString StatName = UEnum::GetDisplayValueAsText(OutOption.StatType).ToString();

			// 퍼센트 여부에 따라 표시 형식 다르게
			if (MainMod.bIsPercentage)
			{
				OutOption.Description = FText::FromString(FString::Printf(TEXT("%s %+.0f%%"), *StatName, MainMod.StackValue));
			}
			else
			{
				OutOption.Description = FText::FromString(FString::Printf(TEXT("%s %+.1f"), *StatName, MainMod.StackValue));
			}
		}
		else
		{
			// [신규]
			OutOption.CurrentStatValue = 0.0f;
			OutOption.NextStatValue = MainMod.BaseValue;

			// 신규 획득은 데이터 테이블 설명을 그대로 사용
			OutOption.Description = Row->Description;
		}
	}
	else
	{
		// 데이터 테이블에 Modifiers가 비어있을 경우 예외 처리
		OutOption.StatType = EItemStatType::None;
		OutOption.Description = Row->Description;
	}
}

EItemGrade URewardSystemComponent::CalculateRandomRarity()
{
	float Total = 0.0f;
	for (auto& Pair : RarityWeights) Total += Pair.Value;

	float Rand = FMath::RandRange(0.0f, Total);
	float Current = 0.0f;

	for (auto& Pair : RarityWeights)
	{
		Current += Pair.Value;
		if (Rand <= Current) return Pair.Key;
	}
	return EItemGrade::Common;
}

void URewardSystemComponent::OnPlayerLevelChanged(float NewLevel)
{
	UE_LOG(LogTemp, Log, TEXT("RewardSystem: 레벨업 감지 (Lv.%.0f) -> 보상 생성 시작"), NewLevel);
	GenerateLevelUpRewards();
}

bool URewardSystemComponent::FindComponents()
{
	// 이미 다 찾아져 있다면 true 반환 (중복 검색 방지)
	if (WeaponComp && InventoryComp && StatusComp) return true;

	AActor* Owner = GetOwner();
	APawn* PlayerPawn = nullptr;

	if (APlayerController* PC = Cast<APlayerController>(Owner))
		PlayerPawn = PC->GetPawn();
	else
		PlayerPawn = Cast<APawn>(Owner);

	if (PlayerPawn)
	{
		WeaponComp = PlayerPawn->FindComponentByClass<UWeaponSystemComponent>();
		InventoryComp = PlayerPawn->FindComponentByClass<UInventoryComponent>();
		StatusComp = PlayerPawn->FindComponentByClass<UStatusComponent>();
	}


	// 셋 다 있어야 true 반환
	return (WeaponComp != nullptr && InventoryComp != nullptr && StatusComp != nullptr);
}

void URewardSystemComponent::SelectReward(const FRewardOption& SelectedOption)
{
	// 컴포넌트 유효성 재확
	if (!WeaponComp || !InventoryComp)
	{
		// 만약 캐싱된게 없다면 다시 찾기 시도
		if (!FindComponents())
		{
			UE_LOG(LogTemp, Error, TEXT("RewardSystem: 컴포넌트가 없어 보상을 지급할 수 없습니다."));
			return;
		}
	}
	float IncrementValue = SelectedOption.NextStatValue - SelectedOption.CurrentStatValue;

	if (SelectedOption.Type == EItemType::Weapon)
	{
		WeaponComp->LevelUpWeapon(SelectedOption.ItemID, SelectedOption.StatType, IncrementValue);
	}
	else
	{
		InventoryComp->AddSecretBook(SelectedOption.ItemID, IncrementValue);
	}

	// 게임 재개
	if (APlayerController* PC = Cast<APlayerController>(GetOwner()))
	{
		PC->SetPause(false);
	}
}

void URewardSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (FindComponents())
	{
		// 2. 이벤트 바인딩
		StatusComp->OnLevelChanged.AddDynamic(this, &URewardSystemComponent::OnPlayerLevelChanged);
		UE_LOG(LogTemp, Log, TEXT("RewardSystem: 초기화 및 이벤트 구독 완료"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RewardSystem: 필수 컴포넌트를 찾지 못했습니다."));
	}
}
