// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MegaBonkGameState.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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

}

bool AMegaBonkGameState::IsOvertime() const
{
	return StageTimer >= StageTimeLimit;
}

void AMegaBonkGameState::BeginPlay()
{
	Super::BeginPlay();
	SpawnProps();
}

float AMegaBonkGameState::GetDisplayGameTime() const
{
	if (IsOvertime())
	{
		// 제한 시간 후: 0초부터 다시 증가 (누적 시간)
		// 예: 현재 305초, 제한 300초 -> 5초 반환
		return StageTimer - StageTimeLimit;
	}
	else
	{
		// 제한 시간 전: 남은 시간 카운트다운
		// 예: 현재 10초, 제한 300초 -> 290초 반환
		return StageTimeLimit - StageTimer;
	}
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

	// ... (기존 데이터 테이블 조회 코드 유지) ...
	static const FString ContextString(TEXT("WaveContext"));
	TArray<FStageWaveInfo*> AllRows;
	WaveDataTable->GetAllRows<FStageWaveInfo>(ContextString, AllRows);

	for (FStageWaveInfo* Row : AllRows)
	{
		// [중요] 데이터 테이블의 시간과 현재 절대 시간 비교
		if (StageTimer >= Row->StartTime && StageTimer < (Row->StartTime + Row->Duration))
		{
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

void AMegaBonkGameState::CheckOvertimeLogic()
{
	// 오버타임 전용 몬스터가 설정되어 있지 않으면 리턴
	if (!OvertimeEnemyClass) return;

	// 오버타임 전용 쿨타임 체크
	if (GetWorld()->GetTimeSeconds() - LastSpawnTime >= OvertimeSpawnInterval)
	{
		// 지정된 마릿수만큼 강력한 몬스터 소환
		for (int32 i = 0; i < OvertimeSpawnAmount; i++)
		{
			SpawnEnemy(OvertimeEnemyClass);
		}
		LastSpawnTime = GetWorld()->GetTimeSeconds();
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

	//반올림 오차 등으로 전체 개수가 모자라면, 첫 번째 항목(보통 가장 흔한 항아리)으로 채웁니다.
	if (SpawnDeck.Num() < TotalPropCount && PropSpawnRules.Num() > 0)
	{
		int32 MissingCount = TotalPropCount - SpawnDeck.Num();
		for (int32 i = 0; i < MissingCount; i++)
		{
			SpawnDeck.Add(PropSpawnRules[0].PropClass);
		}
	}

	// 2.목록을 무작위로 섞습니다. (항아리, 항아리, 상자... 순서를 섞음)
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

	// 3.섞인 목록을 하나씩 꺼내서 맵에 배치합니다.
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

	//제단(Sanctuary) 배치
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

	//보스제단 배치
	for (int32 i = 0; i < BossSpawnerCount; i++)
	{
		if (!BossSpawner) return;

		FVector SpawnLoc;
		if (GetRandomLocationOnNavMesh(SpawnLoc))
		{
			GetWorld()->SpawnActor<AActor>(BossSpawner, SpawnLoc, FRotator::ZeroRotator);
		}
	}

}


bool AMegaBonkGameState::GetRandomLocationOnNavMesh(FVector& OutLocation)
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem) return false;

	//맵의 크기 정의 (예: 가로세로 12000 크기의 사각형 맵이라 가정)
	// BoxExtent는 절반 크기이므로 6000으로 설정
	FVector MapOrigin = FVector::ZeroVector;
	FVector BoxExtent = FVector(6000.0f, 6000.0f, 0.0f); // Z는 무시

	//박스 안에서 완전 랜덤한 지점 생성
	FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(MapOrigin, BoxExtent);

	//그 랜덤 지점을 내비게이션 메쉬 위로 투영 (Project)
	FNavLocation ResultLocation;

	// QueryExtent: 검색할 때 높이 여유값 (Z축으로 얼마나 위아래를 찾을지)
	FVector QueryExtent = FVector(500.0f, 500.0f, 2000.0f);

	// ProjectPointToNavigation: 해당 좌표 수직 아래/위에 NavMesh가 있는지 확인
	if (NavSystem->ProjectPointToNavigation(RandomPoint, ResultLocation, QueryExtent))
	{
		OutLocation = ResultLocation.Location;
		return true;
	}

	return false;
}