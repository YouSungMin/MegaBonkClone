// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyBase.h"
#include "Framework/ObjectPoolSubsystem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::SpawnDrops(UDataTable* DataTable, FName RowName, float ScatterRadius)
{


    if (!DataTable) return;

    // 데이터 조회
    static const FString ContextString(TEXT("Enemy Drop Context"));
    FEnemyData* EnemyData = DataTable->FindRow<FEnemyData>(RowName, ContextString, true);

    if (!EnemyData) return;

    // 개수 계산 (10점당 1개)
    int32 ExpCount = EnemyData->DropExp / 10;
    int32 GoldCount = EnemyData->DropGold / 10;

    // 1. 경험치 스폰 (ExpItemClass 사용)
    if (ExpItemClass)
    {
        for (int32 i = 0; i < ExpCount; i++)
        {
            FVector SpawnLoc = GetActorLocation() + UKismetMathLibrary::RandomUnitVector() * FMath::RandRange(0.0f, ScatterRadius);
            SpawnLoc.Z = GetActorLocation().Z;

            // ResourceType::Exp 설정
            SpawnSinglePickup(ExpItemClass, EResourceType::Exp, 10.0f, SpawnLoc);
        }
    }

    // 2. 골드 스폰 (GoldItemClass 사용)
    if (GoldItemClass)
    {
        for (int32 i = 0; i < GoldCount; i++)
        {
            FVector SpawnLoc = GetActorLocation() + UKismetMathLibrary::RandomUnitVector() * FMath::RandRange(0.0f, ScatterRadius);
            SpawnLoc.Z = GetActorLocation().Z;

            // ResourceType::Gold 설정
            SpawnSinglePickup(GoldItemClass, EResourceType::Gold, 10.0f, SpawnLoc);
        }
    }
}

// 헬퍼 함수 수정: 어떤 클래스(ItemClass)를 스폰할지 받아서 처리
void AEnemyBase::SpawnSinglePickup(TSubclassOf<AResourcePickup> ItemClass, EResourceType Type, float AmountPerItem, FVector Location)
{
    if (!ItemClass) return;

    UWorld* World = GetWorld();
    if (!World) return;

    // [변경] 오브젝트 풀 서브시스템 가져오기
    UObjectPoolSubsystem* PoolSubsystem = World->GetSubsystem<UObjectPoolSubsystem>();
    UE_LOG(LogTemp, Error, TEXT("오브젝트 풀로 생성"));
    if (PoolSubsystem)
    {
        // [변경] 풀 시스템을 통해 액터 가져오기 (없으면 새로 생성해 줌)
        AActor* PooledActor = PoolSubsystem->SpawnPooledActor(ItemClass, Location, FRotator::ZeroRotator, this, nullptr);
        
        // AResourcePickup으로 캐스팅하여 데이터 설정
        AResourcePickup* Pickup = Cast<AResourcePickup>(PooledActor);
        if (Pickup)
        {
            Pickup->ResourceType = Type;
            Pickup->Amount = AmountPerItem;

            // 참고: ObjectPoolSubsystem 내부에서 이미 IObjectPoolInterface::Execute_OnPoolActivate를 호출해 주므로,
            // 별도로 초기화 함수를 부를 필요는 없습니다. (ResourcePickup이 인터페이스를 구현했다면)
        }
    }
    else
    {
        // 만약 서브시스템을 못 찾았다면 비상용으로 일반 스폰 (혹은 로그 출력)
        UE_LOG(LogTemp, Error, TEXT("ObjectPoolSubsystem을 찾을 수 없습니다!"));

        // FActorSpawnParameters SpawnParams;
        // SpawnParams.Owner = this;
        // SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        // World->SpawnActor<AResourcePickup>(ItemClass, Location, FRotator::ZeroRotator, SpawnParams);
    }
}