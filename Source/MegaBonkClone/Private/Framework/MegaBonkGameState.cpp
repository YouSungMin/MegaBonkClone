// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MegaBonkGameState.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
AMegaBonkGameState::AMegaBonkGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	BossSummonCount = 1;
	AliveBossCount = 0;
}

void AMegaBonkGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. 스테이지 타이머 진행
	StageTimer += DeltaTime;

	// 2. 웨이브 체크 및 스폰
	CheckWaveLogic();
}

void AMegaBonkGameState::BeginPlay()
{
	Super::BeginPlay();
	SpawnProps();
}

void AMegaBonkGameState::OnBossDied()
{
	AliveBossCount--;

	// 디버깅용 로그
	UE_LOG(LogTemp, Warning, TEXT("보스 사망! 남은 보스: %d"), AliveBossCount);

	// 남은 보스가 0 이하라면?
	if (AliveBossCount <= 0)
	{
		AliveBossCount = 0; // 혹시 모를 음수 방지

		if (OnAllBossesDead.IsBound())
		{
			OnAllBossesDead.Broadcast();
			UE_LOG(LogTemp, Warning, TEXT("모든 보스 처치 완료! 다음 스테이지 개방 신호 전송!"));
		}
	}
}

void AMegaBonkGameState::CheckWaveLogic()
{
	if (!WaveDataTable) return;

	// 데이터 테이블에서 현재 시간에 맞는 웨이브 정보 가져오기
	static const FString ContextString(TEXT("WaveContext"));
	TArray<FStageWaveInfo*> AllRows;
	WaveDataTable->GetAllRows<FStageWaveInfo>(ContextString, AllRows);

	// 현재 활성화되어야 할 웨이브 찾기
	for (FStageWaveInfo* Row : AllRows)
	{
		if (StageTimer >= Row->StartTime && StageTimer < (Row->StartTime + Row->Duration))
		{
			// 스폰 간격 체크
			// (실제 구현 시에는 각 웨이브별 쿨타임을 따로 관리해야 완벽하지만, 간단한 예시로 글로벌 쿨타임 사용)
			if (GetWorld()->GetTimeSeconds() - LastSpawnTime >= Row->SpawnInterval)
			{
				for (int32 i = 0; i < Row->SpawnAmountPerInterval; i++)
				{
					SpawnEnemy(Row->EnemyClass);
				}
				LastSpawnTime = GetWorld()->GetTimeSeconds();
			}
		}
	}
}

// =========================================================
// [핵심] 랜덤 위치 찾기 & 스폰
// =========================================================
void AMegaBonkGameState::SpawnEnemy(TSubclassOf<AActor> EnemyClass)
{
	// 플레이어 주변에서 스폰 (도넛 모양 범위)
	APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!Player) return;

	FVector Origin = Player->GetActorLocation();
	FVector SpawnLoc;

	// 네비게이션 시스템으로 "갈 수 있는 땅" 찾기
	UNavigationSystemV1* NavSystem= UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem)
	{
	
		FNavLocation ResultLocation;
		// 플레이어 기준 반경 1500~2000 사이 랜덤 위치
		if (NavSystem->GetRandomReachablePointInRadius(Origin, 2000.0f, ResultLocation))
		{
			SpawnLoc = ResultLocation.Location;
			// 너무 가까우면 스킵하는 로직 추가 가능
			GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLoc, FRotator::ZeroRotator);
		}
	}
}

void AMegaBonkGameState::SpawnProps()
{
	// 1. [생성 목록 만들기] 비율에 맞춰서 "무엇을 소환할지" 리스트를 미리 만듭니다.
	TArray<TSubclassOf<AActor>> SpawnDeck;

	int32 CurrentCount = 0;

	for (const FPropSpawnRule& Rule : PropSpawnRules)
	{
		if (!Rule.PropClass) continue;

		// 비율에 따른 개수 계산 (예: 50개 중 10% = 5개)
		int32 CountForThisType = FMath::RoundToInt(TotalPropCount * (Rule.SpawnPercentage / 100.0f));

		for (int32 i = 0; i < CountForThisType; i++)
		{
			SpawnDeck.Add(Rule.PropClass);
		}
		CurrentCount += CountForThisType;
	}

	// [보정] 반올림 오차 등으로 전체 개수가 모자라면, 첫 번째 항목(보통 가장 흔한 항아리)으로 채웁니다.
	if (SpawnDeck.Num() < TotalPropCount && PropSpawnRules.Num() > 0)
	{
		int32 MissingCount = TotalPropCount - SpawnDeck.Num();
		for (int32 i = 0; i < MissingCount; i++)
		{
			SpawnDeck.Add(PropSpawnRules[0].PropClass);
		}
	}

	// 2. [셔플] 목록을 무작위로 섞습니다. (항아리, 항아리, 상자... 순서를 섞음)
	if (SpawnDeck.Num() > 0)
	{
		// Fisher-Yates Shuffle 알고리즘 (언리얼 내장 기능 이용)
		const int32 LastIndex = SpawnDeck.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				SpawnDeck.Swap(i, Index);
			}
		}
	}

	// 3. [배치] 섞인 목록을 하나씩 꺼내서 맵에 배치합니다.
	for (TSubclassOf<AActor> PropClassToSpawn : SpawnDeck)
	{
		FVector SpawnLoc;
		// 네비게이션 위 랜덤 위치 찾기
		if (GetRandomLocationOnNavMesh(SpawnLoc))
		{
			// 위치 찾기에 성공하면 스폰
			GetWorld()->SpawnActor<AActor>(PropClassToSpawn, SpawnLoc, FRotator::ZeroRotator);
		}
		else
		{
			// 위치를 못 찾았을 경우 로그 (원한다면 다시 시도하는 로직 추가 가능)
			// UE_LOG(LogTemp, Warning, TEXT("Failed to find valid location for Prop"));
		}
	}

	// 2. 제단(Sanctuary) 배치
	for (int32 i = 0; i < SanctuaryCount; i++)
	{
		if (Sanctuaries.Num() == 0) break;

		int32 Index = FMath::RandRange(0, Sanctuaries.Num() - 1);
		FVector SpawnLoc;
		if (GetRandomLocationOnNavMesh(SpawnLoc))
		{
			GetWorld()->SpawnActor<AActor>(Sanctuaries[Index], SpawnLoc, FRotator::ZeroRotator);
		}
	}

}

bool AMegaBonkGameState::GetRandomLocationOnNavMesh(FVector& OutLocation)
{
	UNavigationSystemV1* NavSystem= UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem)
	{
		FNavLocation ResultLocation;
		// 맵 전체 범위 (예: 10000) 내에서 랜덤 포인트
		// (Origin은 맵 중앙(0,0,0)으로 가정)
		if (NavSystem->GetRandomReachablePointInRadius(FVector::ZeroVector, 10000.0f, ResultLocation))
		{
			OutLocation = ResultLocation.Location;
			return true;
		}
	}
	return false;
}