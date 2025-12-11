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
	Damage			UMETA(DisplayName = "데미지"),
	AttackSpeed		UMETA(DisplayName = "공격속도"),
	Luck			UMETA(DisplayName = "행운"),
	GoldGain		UMETA(DisplayName = "골드 획득량"),
	HPRegen			UMETA(DisplayName = "체력 재생"),
	MaxHP			UMETA(DisplayName = "최대 체력"),
	EvasionChance	UMETA(DisplayName = "회피율"),
	Difficulty		UMETA(DisplayName = "난이도"),
	MoveSpeed		UMETA(DisplayName = "이동 속도"),
	ExpGain			UMETA(DisplayName = "경험치 획득량"),
	CritChance		UMETA(DisplayName = "치명타 확률"),
	ProjectileCount	UMETA(DisplayName = "발사체 수"),
	JumpPower		UMETA(DisplayName = "점프 높이")
};

// 특수 아이템 효과
UENUM(BlueprintType)
enum class EProcEffectType : uint8
{
	None			UMETA(DisplayName = "없음"),
	Burger			UMETA(DisplayName = "버거(햄버거 생성, HP회복)"),
	Cactus			UMETA(DisplayName = "선인장(피격 시 반사)"),
	CurseDoll		UMETA(DisplayName = "저주 인형(디버프)"),
	Ghost			UMETA(DisplayName = "유령(소환)"),
	IceCrystal		UMETA(DisplayName = "얼음 결정(빙결)"),
	Key				UMETA(DisplayName = "열쇠(상자 무료) "),
	MoldyCheese		UMETA(DisplayName = "곰팡이 치즈(중독)"),
	CampFire		UMETA(DisplayName = "캠프 파이어(소환,HP 회복)"),
	Kebin			UMETA(DisplayName = "케빈(피해 무효)"),
	MoldyGloves		UMETA(DisplayName = "곰팡이 장갑(독구름 생성)"),
	ThunderGloves	UMETA(DisplayName = "천둥 장갑(번개 생성)"),
	Bob				UMETA(DisplayName = "밥 소환(소환)"),
	LooseCannon		UMETA(DisplayName = "루즈 캐논(미사일 발사)"),
	PoisonBarrel	UMETA(DisplayName = "독 배럴(피격시 독구름 생성)"),
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
	float Value = 0.0f;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc", meta = (DisplayName = "아이템 발동 특수 효과"))
	EProcEffectType ProcType = EProcEffectType::None;

	// 발동 확률 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc", meta = (DisplayName = "특수 효과 발동 확률", ClampMin = 0, ClampMax = 1))
	float ProcChance = 0.0f;

	// 발동 시 위력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc", meta = (DisplayName = "특수 효과 적용 값"))
	float ProcValue = 0.0f;

	// 발동 시 스폰할 BP클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc", meta = (DisplayName = "스폰할 액터 클래스 (옵션)"))
	TSoftClassPtr<AActor> SpawnActorClass;

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