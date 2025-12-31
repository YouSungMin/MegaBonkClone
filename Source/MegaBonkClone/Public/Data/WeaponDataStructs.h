// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Data/TypeEnums.h"
#include "WeaponDataStructs.generated.h"
/**
 *
 */

USTRUCT(BlueprintType)
struct FWeaponUpgradeOption
{
	GENERATED_BODY()

public:
	// 강화할 스탯 종류
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "강화 대상 스탯"))
	EWeaponStatType StatType = EWeaponStatType::Damage;

	// 강화 효율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "증가량 (Increment Value)"))
	float Value = 0.0f;
};


 // 아이템 데이터 구조체
USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// ============================================================
	// [기본 정보]
	// ============================================================

	// 무기 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (DisplayName = "무기 이름"))
	FText Name;

	// 무기 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (DisplayName = "무기 설명", MultiLine = "true"))
	FText Description;

	// =========================================================
	// [전투 스탯]
	// =========================================================

	// 기본 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "데미지", ClampMin = "0"))
	float WeaponDamage = 10.0f;

	// 기본 발사체 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "발사체 수", ClampMin = "0"))
	float ProjectileCount = 1;

	// 기본 발사체 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "발사체 속도", ClampMin = "0"))
	float ProjectileSpeed = 1.0f;

	// 기본 발사체 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "발사체 크기", ClampMin = "0"))
	float ProjectileScale = 0.0f;

	// 기본 발사체 반사
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "발사체 반사", ClampMin = "0"))
	float ChainCount = 0.0f;

	// 치명타 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "치명타 확률", ClampMin = "0"))
	float CriticalChance = 0;

	// 치명타 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "치명타 데미지", ClampMin = "0"))
	float CriticalDamage = 0.0f;

	// 기본 넉백
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "넉백", ClampMin = "0"))
	float KnockBack = 0.0f;

	// 쿨타임 감소
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "쿨타임", ClampMin = "0"))
	float Cooldown = 0.0f;

	// 강화 효율 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades", meta = (DisplayName = "강화 선택지 목록 (Pool)", TitleProperty = "StatType"))
	TArray<FWeaponUpgradeOption> UpgradeOptions;

	// 생성될 BP 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class", meta = (DisplayName = "무기 액터 클래스"))
	TSubclassOf<AActor> WeaponClass;

	// =========================================================
	// [리소스]
	// =========================================================
	// 아이콘 이미지 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual", meta = (DisplayName = "아이콘"))
	TSoftObjectPtr<UTexture2D> Icon;
};