// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemyDataStructs.generated.h"
/**
 *
 */

 // 몬스터 등급
UENUM(BlueprintType)
enum class EEnemyRank : uint8
{
	None,
	Normal		UMETA(DisplayName = "일반"),
	Elite		UMETA(DisplayName = "엘리트"),
	Boss		UMETA(DisplayName = "보스")
};

// 몬스터 데이터 구조체
USTRUCT(BlueprintType)
struct FEnemyData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// ============================================================
	// [기본 정보]
	// ============================================================

	// 몬스터 이름 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (DisplayName = "몬스터 이름"))
	FText Name;

	// 몬스터 등급 ( 일반, 엘리트, 보스 ) 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (DisplayName = "몬스터 등급"))
	EEnemyRank Rank = EEnemyRank::None;

	// ============================================================
	// [전투 스탯]
	// ============================================================

	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "몬스터 최대체력"))
	float MaxHP = 100.0f;

	// 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "몬스터 공격력"))
	float AttackPower = 10.0f;

	// 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "아이템 이동속도"))
	float MoveSpeed = 10.0f;

	// -- 보상 --

	// 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward", meta = (DisplayName = "드랍 경험치"))
	int32 DropExp = 10;

	// ============================================================
	// [리소스]
	// ============================================================

	// Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual", meta = (DisplayName = "몬스터 Mesh"))
	TSoftObjectPtr<USkeletalMesh> Mesh;

	// AnimInstancee 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual", meta = (DisplayName = "몬스터 AnimClass"))
	TSoftObjectPtr<UAnimInstance> AnimClass;

	// AI Controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual", meta = (DisplayName = "몬스터 ControllerClass"))
	TSoftObjectPtr<AController> AIControllerClass;
};