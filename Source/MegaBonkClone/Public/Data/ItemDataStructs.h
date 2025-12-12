// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStructs.generated.h"
/**
 * 
 */

// 아이템 등급
UENUM(BlueprintType)
enum class EItemType : uint8
{
	GeneralItem		UMETA(DisplayName = "일반 아이템"), // 갯수 제한 없이 중첩 가능
	SecretBook		UMETA(DisplayName = "비전서") // 최대 4개까지 장착 가능
};

// 아이템 등급
UENUM(BlueprintType)
enum class EItemGrade : uint8
{
	None,
	Common			UMETA(DisplayName = "일반 (Common)"),
	UnCommon		UMETA(DisplayName = "고급 (UnCommon)"),
	Rare			UMETA(DisplayName = "희귀 (Rare)"),
	Legendary		UMETA(DisplayName = "전설 (Legendary)")
};

// 스탯 유형
UENUM(BlueprintType)
enum class EItemStatType : uint8
{
	None			UMETA(DisplayName = "없음"),
	MaxHP			UMETA(DisplayName = "최대 체력"),
	HPRegen			UMETA(DisplayName = "체력 재생"),
	OverHeal		UMETA(DisplayName = "오버 힐"),
	Armor			UMETA(DisplayName = "방어력"),
	EvasionChance	UMETA(DisplayName = "회피율"),
	LifeDrain		UMETA(DisplayName = "생명 흡수"),
	Thorn			UMETA(DisplayName = "가시"),
	ExpGain			UMETA(DisplayName = "경험치 획득량"),
	GoldGain		UMETA(DisplayName = "골드 획득량"),
	Difficulty		UMETA(DisplayName = "난이도"),
	Damage			UMETA(DisplayName = "데미지"),
	CritChance		UMETA(DisplayName = "치명타 확률"),
	AttackSpeed		UMETA(DisplayName = "공격속도"),
	ProjectileCount	UMETA(DisplayName = "발사체 수"),
	KnockBack		UMETA(DisplayName = "넉백"),
	MoveSpeed		UMETA(DisplayName = "이동 속도"),
	JumpPower		UMETA(DisplayName = "점프 높이"),
	Luck			UMETA(DisplayName = "행운")
};

// 특수 효과 유형
UENUM(BlueprintType)
enum class EProcEffectType : uint8
{
	None			UMETA(DisplayName = "없음"),
	SpawnActor		UMETA(DisplayName = "액터 소환"),
	FireProjectile	UMETA(DisplayName = "투사체 발사"),
	ApplyStatus		UMETA(DisplayName = "상태 이상 부여"),
	AreaExplosion	UMETA(DisplayName = "범위 공격"),
	ReflectDamage	UMETA(DisplayName = "반사"),
	TimeStop		UMETA(DisplayName = "시간 정지"),
	Execute			UMETA(DisplayName = "처형")
};

UENUM(BlueprintType)
enum class EStatusEffectType : uint8
{
	None,
	Freeze		UMETA(DisplayName = "빙결"),
	Poison		UMETA(DisplayName = "독/중독"),
	Burn		UMETA(DisplayName = "화상"),
	Shock		UMETA(DisplayName = "감전"),
	Bleed		UMETA(DisplayName = "출혈")
};

USTRUCT(BlueprintType)
struct FItemProcData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "행동 타입"))
	EProcEffectType ProcType = EProcEffectType::None;

	// [추가] 상태 이상 타입 (ProcType이 ApplyStatus일 때만 사용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "상태 이상 종류 (옵션)"))
	EStatusEffectType StatusType = EStatusEffectType::None;

	// 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "발동 확률 (%)"))
	float BaseChance = 0.0f;

	// 중첩당 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "중첩당 확률 증가 (%)"))
	float StackChance = 0.0f;

	// 위력 (Value)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "기본 위력 (Value)"))
	float BaseValue = 0.0f;

	// 중첩 당 위력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "중첩당 위력 증가"))
	float StackValue = 0.0f;

	// 기타 옵션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "쿨타임 (Cooldown)"))
	float Cooldown = 0.0f;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "수치"))
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