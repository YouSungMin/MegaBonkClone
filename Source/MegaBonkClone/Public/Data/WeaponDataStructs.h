// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponDataStructs.generated.h"
/**
 *
 */

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

	// 기본 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "공격력", ClampMin = "0"))
	float AttackPower = 10.0f;

	// 기본 공격 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "공격 속도", ClampMin = "0"))
	float AttackSpeed = 1.0f;

	// =========================================================
	// [리소스]
	// =========================================================
	// 아이콘 이미지 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual", meta = (DisplayName = "아이콘"))
	TSoftObjectPtr<UTexture2D> Icon;
};