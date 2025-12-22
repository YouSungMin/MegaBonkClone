// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/BossSpawnerActor.h"
#include "Framework/MegaBonkGameState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossSpawnerActor::ABossSpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	BossSpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BossSpawnerMesh->SetupAttachment(Root);
	BossSpawnerMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	BossSpawnerMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

// Called when the game starts or when spawned
void ABossSpawnerActor::BeginPlay()
{
	Super::BeginPlay();
	GameState = GetWorld()->GetGameState<AMegaBonkGameState>();

	if (GameState)
	{
		GameState->OnAllBossesDead.AddDynamic(this, &ABossSpawnerActor::OnBossClear);
		UE_LOG(LogTemp, Warning, TEXT("바인딩 성공"));
	}
}

void ABossSpawnerActor::Interact_Implementation(AActor* PlayerActor)
{
	// 보스 소환 마리 수를 담은 게임 스테이트 클래스 형변환 

	switch (CurrentState)
	{
	case EAltarState::ReadyToSummon:
		if (GameState)
		{
			SpawnBosses(GameState->GetBossSummonCount());
			CurrentState = EAltarState::BossesAlive;
		}
		break;
	case EAltarState::BossesAlive:
		UE_LOG(LogTemp, Warning, TEXT("아직 보스가 살아있음"));
		break;
	case EAltarState::StageClear:
		// 다음 스테이지로 넘어가는 함수
		UE_LOG(LogTemp, Warning, TEXT("다음 스테이지 이동"));
		break;
	default:
		break;
	}
}

void ABossSpawnerActor::SpawnBosses(int32 Amount)
{
	for (int i = 0; i < Amount; i++)
	{
		// 보스 소환 함수
		// EnemyDataTable에서 보스 데이터를 넘겨 받아서 스폰할듯 
		UE_LOG(LogTemp, Log, TEXT("보스 %d 마리 소환 "), GameState->GetBossSummonCount());
	}
	GameState->AddAliveBossCount(Amount);
}

void ABossSpawnerActor::OnBossClear()
{
	CurrentState = EAltarState::StageClear;
	UE_LOG(LogTemp,Log,TEXT("보스 클리어, 다음 스테이지 활성화"));
}


