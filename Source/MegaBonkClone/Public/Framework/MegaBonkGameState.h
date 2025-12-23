// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Data/StageDataStructs.h"
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
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

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

private:
	void CheckWaveLogic();
	void SpawnEnemy(TSubclassOf<AActor> EnemyClass);

	// 랜덤 위치 찾기 (NavMesh 사용)
	bool GetRandomLocationOnNavMesh(FVector& OutLocation);

	// 소품(상자, 항아리) 배치 함수
	void SpawnProps();
protected:
	// 보스 소환 스택 (외부에서 직접 수정 못하게 protected로 선언)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Mechanics")
	int32 BossSummonCount;

	// 현재 살아있는 보스 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Mechanics")
	int32 AliveBossCount;

	// [스테이지 설정]
	UPROPERTY(EditDefaultsOnly, Category = "Stage")
	UDataTable* WaveDataTable; // 웨이브 데이터 테이블 할당

	// [오브젝트 배치 설정]
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TArray<TSubclassOf<AActor>> RandomProps; // 항아리, 상자 클래스 목록

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TArray<TSubclassOf<AActor>> Sanctuaries; // 제단 클래스 목록

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	int32 PropCount = 50; // 맵에 뿌릴 항아리 개수

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	int32 SanctuaryCount = 5; // 맵에 뿌릴 제단 개수

	// [현재 상태]
	UPROPERTY(BlueprintReadOnly, Category = "Stage")
	float StageTimer = 0.0f;

private:
	

	// 웨이브 관리 변수
	float LastSpawnTime = 0.0f;
	TArray<FStageWaveInfo*> CurrentWaves;
};
