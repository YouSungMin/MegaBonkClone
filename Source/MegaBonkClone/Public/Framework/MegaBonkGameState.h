// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Data/StageDataStructs.h"
#include "MegaBonkGameState.generated.h"

// 보스가 다 죽었을때 보낼 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllBossesDead);


USTRUCT(BlueprintType)
struct FPropSpawnRule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PropClass; // 생성할 액터 (예: 항아리, 상자)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float SpawnPercentage; // 확률 (0~100%)
};
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

	// 제한 시간이 지났는지 확인하는 함수
	UFUNCTION(BlueprintCallable, Category = "Stage")
	bool IsOvertime() const;

	// UI에 표시할 시간을 반환하는 함수 (자동으로 카운트다운/업 전환)
	UFUNCTION(BlueprintCallable, Category = "Stage")
	float GetDisplayGameTime() const;

	UPROPERTY(BlueprintAssignable, Category = "Boss Mechanics")
	FOnAllBossesDead OnAllBossesDead;

private:
	void CheckWaveLogic();
	void CheckOvertimeLogic();  // 오버타임 웨이브 (고정 몹)
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

	//비율 설정을 위한 변수
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TArray<FPropSpawnRule> PropSpawnRules;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	int32 TotalPropCount = 50; // 전체 생성 개수

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TArray<TSubclassOf<AActor>> Sanctuaries; // 제단 클래스 목록

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	int32 SanctuaryCount = 5; // 맵에 뿌릴 제단 개수

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> BossSpawner = nullptr; // 보스제단 클래스 목록

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	int32 BossSpawnerCount = 1; 

	UPROPERTY(EditDefaultsOnly, Category = "Overtime")
	TSubclassOf<AActor> OvertimeEnemyClass; // 예: 강력한 엘리트 몹

	UPROPERTY(EditDefaultsOnly, Category = "Overtime")
	float OvertimeSpawnInterval = 1.0f; // 소환 간격

	UPROPERTY(EditDefaultsOnly, Category = "Overtime")
	int32 OvertimeSpawnAmount = 5;

	// [현재 상태]
	UPROPERTY(BlueprintReadOnly, Category = "Stage")
	float StageTimer = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stage")
	float StageTimeLimit = 600.0f;
private:
	

	// 웨이브 관리 변수
	float LastSpawnTime = 0.0f;
	TArray<FStageWaveInfo*> CurrentWaves;
};
