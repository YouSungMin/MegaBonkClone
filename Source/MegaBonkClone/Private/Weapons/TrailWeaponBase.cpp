// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/TrailWeaponBase.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Characters/Components/StatusComponent.h"
#include "Framework/ObjectPoolSubsystem.h"
#include "Weapons/TrailWeaponActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATrailWeaponBase::ATrailWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATrailWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}



void ATrailWeaponBase::AttackWeapon_Implementation()
{
	if (!TrailClass) return;
	if (!OwnerCharacter.IsValid()) return;

	Super::AttackWeapon_Implementation();

	//기본 위치는 플레이어 위치
	FVector TraceStart = OwnerCharacter->GetActorLocation();
	FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 1000.0f); // 캐릭터 아래 1000유닛(10m)까지 탐색
	FVector SpawnLocation = TraceStart;
	FRotator SpawnRotation = FRotator::ZeroRotator;

	//바닥 탐지 (라인 트레이스)
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter.Get());//내 캐릭터는 무시 (발바닥 충돌 방지)

	// ECC_WorldStatic: 고정된 지형/벽 등과 충돌 검사 (필요에 따라 ECC_Visibility 등으로 변경 가능)
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_WorldStatic,
		Params
	);

	if (bHit)
	{
		// 바닥에 닿았다면 그 위치를 스폰 위치로 사용
		SpawnLocation = HitResult.Location;
		SpawnRotation = FRotationMatrix::MakeFromZ(HitResult.ImpactNormal).Rotator();
		//바닥과 완벽하게 겹쳐서 깜빡이는 현상(Z-Fighting) 방지를 위해 살짝 위로 띄움
		SpawnLocation.Z += 2.0f;
	}
	else
	{
		// 공중이거나 바닥이 없으면 기존처럼 0으로 하거나 캐릭터 위치 사용
		SpawnLocation.Z = 0.0f;
	}


	//오브젝트 풀 서브시스템 사용
	UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	if (!Pool) return;

	//장판 생성 (SpawnPooledActor 사용) - spawnLoction 대신 새로 구한 SpawnLocation 사용
	ATrailWeaponActor* NewTrail = Pool->SpawnPoolActor<ATrailWeaponActor>(
		TrailClass,
		SpawnLocation,
		SpawnRotation,
		this, // Owner
		Cast<APawn>(OwnerCharacter.Get()) // Instigator
	);

	if (NewTrail)
	{
		float totalCriticalChance = CriticalChance + OwnerStatusComp->GetResultCriticalChance();
		// (데미지, 지속시간, 크기, 공격속도)
		UE_LOG(LogTemp, Warning, TEXT("Duration: %.1f"), FinalDuration);
		NewTrail->InitializeTrail(WeaponFinalDamage, WeaponFinalCriticalDamage, totalCriticalChance, FinalDuration, AttackSize, CurrentAttackInterval);
	}
}

