// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StageDataStructs.generated.h"

USTRUCT(BlueprintType)
struct FStageWaveInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartTime; // 시작 시간 (초 단위, 예: 60 = 1분)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;  // 지속 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> EnemyClass; // 스폰할 몬스터 클래스

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnInterval; // 스폰 간격 (예: 0.5초마다)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnAmountPerInterval; // 한 번에 나올 마릿수
};
/**
 * 
 */
