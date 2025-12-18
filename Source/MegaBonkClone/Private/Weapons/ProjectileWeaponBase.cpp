// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeaponBase.h"
#include "Weapons/ProjectileWeaponActor.h"
#include "Framework/ObjectPoolSubsystem.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Characters/Components/StatusComponent.h"
#include "Weapons/BouncingProjectileWeaponActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileWeaponBase::AProjectileWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AProjectileWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileWeaponBase::AttackWeapon_Implementation()
{

	if (!ProjectileClass) return;

	// 1. 공격 시작 시 스탯 최신화 (한 번만 계산)
	UpdateWeaponStats();

	// 2. 발사해야 할 총 개수 설정
	BurstShotsLeft = FMath::Max(1, FMath::FloorToInt(FinalProjectileCount));

	// 3. 연사 간격 (Delay) 설정 (예: 0.1초마다 발사)
	// 필요하다면 WeaponData에 'BurstInterval' 같은 변수를 추가해서 쓰셔도 됩니다.
	float BurstDelay = 0.1f;

	// 4. 타이머 시작 (즉시 첫 발 발사하고, 0.1초마다 반복)
	GetWorldTimerManager().SetTimer(
		BurstTimerHandle,
		this,
		&AProjectileWeaponBase::FireSingleProjectile,
		BurstDelay,
		true, // 반복(Loop) 활성화
		0.0f  // 첫 발은 대기 없이 0초에 즉시 발사
	);
	
}

void AProjectileWeaponBase::FireSingleProjectile()
{
	// 남은 횟수가 없으면 타이머 종료
	if (BurstShotsLeft <= 0)
	{
		GetWorldTimerManager().ClearTimer(BurstTimerHandle);
		return;
	}

	// 횟수 차감
	BurstShotsLeft--;

	

	// 매 발사마다 타겟 다시 찾기
	FVector SpawnLoc = OwnerCharacter.Get()->GetActorLocation();
	FRotator SpawnRot = OwnerCharacter.Get()->GetActorRotation();

	AActor* NearestEnemy = FindNearestEnemy();

	// 타겟팅 모드일 때 적 없으면 쏘지 않음 (취향에 따라 삭제 가능)
	if (bOnlyFireWhenTargetFound && NearestEnemy == nullptr)
	{
		// 적이 없어서 안 쐈어도 횟수는 까는 게 일반적이지만, 
		// 아끼고 싶으면 BurstShotsLeft++ 하셔도 됩니다.
		return;
	}

	if (NearestEnemy)
	{
		FVector Dir = NearestEnemy->GetActorLocation() - SpawnLoc;
		SpawnRot = Dir.Rotation();
	}

	UObjectPoolSubsystem* poolSystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();

	//스폰 
	AProjectileWeaponActor* NewProj = poolSystem->SpawnPoolActor<AProjectileWeaponActor>(
		ProjectileClass,
		SpawnLoc,
		SpawnRot,
		this,
		Cast<APawn>(OwnerCharacter.Get())
	);

	//데이터 주입
	if (NewProj)
	{
		// WeaponBase 함수 사용하지 않고 직접 확률 계산 (Per-Hit를 위해 확률 전달)
		float TotalCritChance = CriticalChance;
		if (OwnerStatusComp.IsValid()) {
			TotalCritChance += OwnerStatusComp->GetResultCriticalChance();
		}

		// 확률(TotalCritChance)을 인자로 넘겨줌 (ProjectileActor 수정본 기준)
		NewProj->InitializeProjectile(
			WeaponFinalDamage,         // 일반 뎀
			WeaponFinalCriticalDamage, // 크리 뎀
			TotalCritChance,           // 확률
			FinalProjectileSpeed,
			5.0f,
			KnockBack,
			false
		);

		if (FinalAttackSize > 0.0f)
		{
			NewProj->SetActorScale3D(FVector(FinalAttackSize));
		}

		if (ABouncingProjectileWeaponActor* BouncingProj = Cast<ABouncingProjectileWeaponActor>(NewProj))
		{
			BouncingProj->InitializeBouncing(FMath::FloorToInt(FinalReflectCount), 1000.0f);
		}
	}
}





