// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Data/TypeEnums.h"
#include "ItemDataStructs.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct FItemProcData
{
	GENERATED_BODY()

public:
	// 발동 조건
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "발동 조건 (Trigger)"))
	EProcTriggerType TriggerType = EProcTriggerType::OnHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "행동 타입"))
	EProcEffectType ProcType = EProcEffectType::None;

	// 변경할 스탯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "대상 스탯 (Buff용)"))
	EItemStatType StatType = EItemStatType::None;

	// 퍼센트 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "퍼센트(%) 적용"))
	bool bIsPercentage = false;

	// 상태 이상 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "상태 이상 종류 (옵션)"))
	EStatusEffectType StatusType = EStatusEffectType::None;

	// 발동 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "발동 확률 (%)"))
	float BaseChance = 0.0f;

	// 중첩당 발동 확률 증가율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "중첩당 확률 증가 (%)"))
	float StackChance = 0.0f;

	// 위력 (Value)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "기본 위력 (BaseValue)"))
	float BaseValue = 0.0f;

	// 중첩 당 위력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "중첩당 증가값"))
	float StackValue = 0.0f;

	// 기타 옵션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "쿨타임 (Cooldown)"))
	float Cooldown = 0.0f;

	// 지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "지속 시간 (Duration)"))
	float Duration = 0.0f;

	// 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "범위 (Radius)"))
	float Radius = 0.0f;

	// 소환할 액터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "스폰할 액터 클래스"))
	TSoftClassPtr<AActor> SpawnActorClass;
};

// 스탯 변경 정보 하나를 담는 구조체
USTRUCT(BlueprintType)
struct FItemStatModifier
{
	GENERATED_BODY()

public:
	// 변경할 스탯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "스탯 타입"))
	EItemStatType StatType = EItemStatType::None;

	// 변경 수치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "기본 위력 (BaseValue)"))
	float BaseValue = 0.0f;

	// 중첩당 수치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "중첩당 증가값"))
	float StackValue = 0.0f;

	// 퍼센트 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "퍼센트(%) 적용"))
	bool bIsPercentage = false;
};

// 아이템 데이터 구조체
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// ============================================================
	// [기본 정보]
	// ============================================================

	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (DisplayName = "아이템 이름"))
	FText Name;

	// 아이템 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (DisplayName = "아이템 설명", MultiLine = "true"))
	FText Description;

	// 아이템 등급
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (DisplayName = "등급"))
	EItemGrade Grade = EItemGrade::Common;

	// 아이템 유형( 비전서 / 일반아이템)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (DisplayName = "아이템 유형"))
	EItemType ItemType = EItemType::GeneralItem;

	// =========================================================
	// [전투 스탯]
	// =========================================================

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Effect", meta = (DisplayName = "스탯 옵션 목록", TitleProperty = "StatType"))
	TArray<FItemStatModifier> Modifiers;

	// =========================================================
	// [특수 발동 효과 (Proc)]
	// =========================================================
	
	// 아이템 발동할 특수 효과
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc", meta = (DisplayName = "특수 효과 목록", TitleProperty = "ProcType"))
	TArray<FItemProcData> ProcModifiers;

	// =========================================================
	// [상인 거래 정보]
	// =========================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy", meta = (DisplayName = "가격 (상인 기준)", ClampMin = "0"))
	int32 Price = 0;

	// =========================================================
	// [리소스]
	// =========================================================
	// 아이콘 이미지 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual", meta = (DisplayName = "아이콘"))
	TSoftObjectPtr<UTexture2D> Icon;
};