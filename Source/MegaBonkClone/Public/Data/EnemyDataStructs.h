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
	// -- 기본 정보 --

	// 적 이름 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FText Name;

	// 적 타입 ( 일반, 엘리트, 보스 ) 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	EEnemyRank Rank;

	// -- 전투 스텟 --
	
	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EEnemyRank MaxHP;

	// 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EEnemyRank AttackPower;

	// 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EEnemyRank MoveSpeed;

	// -- 보상 --

	// 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	int32 DropExp;

	// -- 리소스 --

	// Mesh
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	//int32 DropExp;
	//
	//// 
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	//int32 DropExp;

	////
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	//int32 DropExp;
};