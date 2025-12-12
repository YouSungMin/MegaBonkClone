// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/TrailWeaponSpawnActor.h"
#include "Weapons/TrailWeaponActor.h"
// Sets default values
ATrailWeaponSpawnActor::ATrailWeaponSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BaseRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BaseRoot"));

	SetRootComponent(BaseRoot);
}

// Called when the game starts or when spawned
void ATrailWeaponSpawnActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&ATrailWeaponSpawnActor::SpawnTrailWeapon,
		1.0f,
		true
	);
}

void ATrailWeaponSpawnActor::SpawnTrailWeapon()
{
	// TrailClass가 할당되었는지 체크 (안전장치)
	if (TrailClass)
	{
		// 충돌 때문에 스폰 실패하는 것을 방지 (선택 사항)
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<ATrailWeaponActor>(
			TrailClass,
			FVector(GetActorLocation().X, GetActorLocation().Y,0.0f),
			GetActorRotation(),
			SpawnParams
		);
	}
}


