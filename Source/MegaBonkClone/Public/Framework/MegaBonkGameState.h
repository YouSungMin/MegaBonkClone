// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MegaBonkGameState.generated.h"

// 보스가 다 죽었을때 보낼 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllBossesDead);

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API AMegaBonkGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMegaBonkGameState(); 

	// 보스 소환를 1 증가시키는 함수
	UFUNCTION(BlueprintCallable, Category = "Boss Spawner")
	inline void IncreaseBossSummonCount(){ BossSummonCount++;}

	// 현재 보스 소환 카운트를 가져오는 함수 (Getter)
	UFUNCTION(BlueprintPure, Category = "Boss Spawner")
	inline int32 GetBossSummonCount() const{ return BossSummonCount;}

	// 보스가 소환될 때 호출 (예: 3마리 소환하면 Amount에 3 입력)
	UFUNCTION(BlueprintCallable, Category = "Boss Mechanics")
	inline void AddAliveBossCount(int32 Amount){ AliveBossCount += Amount; }

	// 보스가 죽을 때 호출
	UFUNCTION(BlueprintCallable, Category = "Boss Mechanics")
	void OnBossDied();

	UPROPERTY(BlueprintAssignable, Category = "Boss Mechanics")
	FOnAllBossesDead OnAllBossesDead;
protected:
	// 보스 소환 스택 (외부에서 직접 수정 못하게 protected로 선언)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Mechanics")
	int32 BossSummonCount;

	// 현재 살아있는 보스 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Mechanics")
	int32 AliveBossCount;
};
