
// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/BossSpawnerActor.h"
#include "Framework/MegaBonkGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
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
	// 위젯 컴포넌트 생성 및 설정
	InteractionWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidgetComp->SetupAttachment(RootComponent); // 루트에 붙이기

	// Screen 모드로 하면 카메라를 항상 정면으로 바라보고 크기가 일정하게 유지됨 (빌보드)
	InteractionWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	// 위젯 크기 설정 (원하는 대로 조절)
	InteractionWidgetComp->SetDrawAtDesiredSize(true);

	// 위치를 아이템 머리 위로 살짝 올림 (Z축 +80)
	InteractionWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));

	// 처음엔 안 보이게 숨김
	InteractionWidgetComp->SetVisibility(false);
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
	if (!BossClass)
	{
		UE_LOG(LogTemp, Error, TEXT("BossClass가 할당되지 않음"));
		return;
	}
	float MinRadius = 400.0f;
	float MaxRadius = 800.0f;

	for (int i = 0; i < Amount; i++)
	{
		// 보스 소환 함수

		FVector RandomDir = FMath::VRand();
		RandomDir.Z = 0.0f;
		RandomDir.Normalize();

		// 2. 최소~최대 반경 사이의 랜덤 거리 구하기
		float RandomDist = FMath::RandRange(MinRadius, MaxRadius);

		// 3. 최종 위치 계산 (스포너 위치 + (방향 * 거리))
		FVector SpawnLocation = GetActorLocation() + (RandomDir * RandomDist);

		// 바닥에 살짝 띄우기 (너무 높으면 NavMesh 못 찾을 수 있으니 적당히)
		SpawnLocation.Z = GetActorLocation().Z + 100.0f;

		FRotator SpawnRotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; // 겹쳐도 일단 소환

		// 실제 보스 소환
		AActor* NewBoss = GetWorld()->SpawnActor<AActor>(BossClass, SpawnLocation, SpawnRotation, SpawnParams);

		if (NewBoss)
		{
			UE_LOG(LogTemp, Log, TEXT("보스 %d 마리 소환 성공: %s"), (i + 1), *NewBoss->GetName());
		}
	}
	UE_LOG(LogTemp, Log, TEXT("보스 %d 마리 소환 "), GameState->GetBossSummonCount());
	GameState->AddAliveBossCount(Amount);
}

void ABossSpawnerActor::OnBossClear()
{
	CurrentState = EAltarState::StageClear;
	UE_LOG(LogTemp,Log,TEXT("보스 클리어, 다음 스테이지 활성화"));
}


void ABossSpawnerActor::BeginFocus_Implementation()
{
	if (InteractionWidgetComp)
	{
		InteractionWidgetComp->SetVisibility(true);
	}
}

void ABossSpawnerActor::EndFocus_Implementation()
{
	if (InteractionWidgetComp)
	{
		InteractionWidgetComp->SetVisibility(false);
	}
}
