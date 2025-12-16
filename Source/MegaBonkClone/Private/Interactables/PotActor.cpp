// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/PotActor.h"
#include "Kismet/GameplayStatics.h"
#include "Items/PickupItem/ResourcePickup.h"

// Sets default values
APotActor::APotActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PotMesh"));
	SetRootComponent(PotMesh);
	PotMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

// Called when the game starts or when spawned
void APotActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APotActor::Interact_Implementation(AActor* InstigatorActor)
{
	// 깨지는 이펙트 (파티클) 재생
	if (BreakEffect)
	{

	}

	// 깨지는 소리 재생
	if (BreakSound)
	{

	}

	if (DropPickup.Num() > 0)
	{
		int32 dropCount = FMath::RandRange(MinDropCount, MaxDropCount);

		for (int32 i = 0; i < dropCount; i++)
		{
			int32 RandomIndex = FMath::RandRange(0, DropPickup.Num()-1);
			TSubclassOf<AResourcePickup> ItemClassToSpawn = DropPickup[RandomIndex];

			if (ItemClassToSpawn)
			{
				FVector SpawnLocation = GetActorLocation() + FMath::VRand() * FMath::RandRange(20.0f, 50.0f);
				SpawnLocation.Z = GetActorLocation().Z + 20.0f; // 바닥에 박히지 않게 살짝 위로

				FRotator SpawnRotation = FRotator::ZeroRotator;

				// 충돌 처리 옵션
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				// 아이템 스폰
				AResourcePickup* SpawnedItem = GetWorld()->SpawnActor<AResourcePickup>(ItemClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

				if (SpawnedItem)
				{
					// ★ 핵심: 생성되자마자 플레이어(InstigatorActor)가 획득한 것으로 처리
					// InstigatorActor는 항아리를 깬 주체(플레이어)입니다.
					if (SpawnedItem->GetClass()->ImplementsInterface(UPickupInterface::StaticClass()))
					{
						IPickupInterface::Execute_OnPickup(SpawnedItem, InstigatorActor);
					}
				}

			}
		}
	}

	Destroy();
}

