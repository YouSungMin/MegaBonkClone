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
enum class EItemGrade : uint8
{
	None,
	Common			UMETA(DisplayName = "일반 (Common)"),
	UnCommon		UMETA(DisplayName = "고급 (UnCommon)"),
	Rare			UMETA(DisplayName = "희귀 (Rare)"),
	Epic			UMETA(DisplayName = "영웅 (Epic)"),
	Legendary		UMETA(DisplayName = "전설 (Legendary)")
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info", meta = (DisplayName = "아이템 이름"))
	FText Name;

	// 아이템 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info", meta = (DisplayName = "아이템 설명", MultiLine = "true"))
	FText Description;

	// 아이템 등급
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info", meta = (DisplayName = "등급"))
	EItemGrade Grade = EItemGrade::Common;

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