// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyBase.h"
#include "Framework/ObjectPoolSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/DamageTextActor.h"

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

void AEnemyBase::ShowDamagePopup(float DamageAmount)
{
    // 0 이라거나 잘못된 값은 무시
    if (DamageAmount <= 0.0f) return;

    UWorld* World = GetWorld();
    if (!World) return;

    // 풀 서브시스템 가져오기
    UObjectPoolSubsystem* Pool = World->GetSubsystem<UObjectPoolSubsystem>();

    // 풀 시스템이 있고, BP_DamageTextActor 클래스가 설정되어 있어야 함
    if (Pool && DamageTextActorClass)
    {
        //머리 위 위치 계산 (캡슐 높이 + 50 정도)
        float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
        // 필요하다면 좌우로 조금 랜덤하게 퍼지게 할 수도 있음
        FVector SpawnLocation = GetActorLocation() + FVector(0, 0, CapsuleHalfHeight + 50.0f);

        //풀에서 꺼내기 (없으면 서브시스템이 알아서 생성함)
        AActor* PooledActor = Pool->SpawnPooledActor(DamageTextActorClass, SpawnLocation, FRotator::ZeroRotator, this, nullptr);

        //데미지 수치 전달
        if (ADamageTextActor* DmgActor = Cast<ADamageTextActor>(PooledActor))
        {
            DmgActor->SetDamageValue(DamageAmount);
        }
    }
}

// 헬퍼 함수 수정: 어떤 클래스(ItemClass)를 스폰할지 받아서 처리
void AEnemyBase::SpawnSinglePickup(TSubclassOf<AResourcePickup> ItemClass, EResourceType Type, float AmountPerItem, FVector Location)
{
    if (!ItemClass) return;

    UWorld* World = GetWorld();
    if (!World) return;


    UObjectPoolSubsystem* PoolSubsystem = World->GetSubsystem<UObjectPoolSubsystem>();
    UE_LOG(LogTemp, Error, TEXT("오브젝트 풀로 생성"));
    if (PoolSubsystem)
    {
        AActor* PooledActor = PoolSubsystem->SpawnPooledActor(ItemClass, Location, FRotator::ZeroRotator, this, nullptr);
        
        // AResourcePickup으로 캐스팅하여 데이터 설정
        AResourcePickup* Pickup = Cast<AResourcePickup>(PooledActor);
        if (Pickup)
        {
            Pickup->ResourceType = Type;
            Pickup->Amount = AmountPerItem;
        }
    }
 
}