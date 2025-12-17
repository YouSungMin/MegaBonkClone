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

	FVector spawnLoction = OwnerCharacter->GetActorLocation();
	spawnLoction.Z = 0.0f; // 바닥 높이 고정 (필요 시 수정)

	FRotator SpawnRot = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

	// [수정] 오브젝트 풀 서브시스템 사용
	UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	if (!Pool) return;

	// 3. 장판 생성 (SpawnPooledActor 사용)
	ATrailWeaponActor* NewTrail = Pool->SpawnPoolActor<ATrailWeaponActor>(
		TrailClass,
		spawnLoction,
		SpawnRot,
		this, // Owner
		Cast<APawn>(OwnerCharacter.Get()) // Instigator
	);

	if (NewTrail)
	{
		// 4. 데이터 주입
		float FinalDamage = WeaponFinalDamage;
		float Size = ProjectileAttackSize > 0.0f ? ProjectileAttackSize : 1.0f;
		float Duration = OwnerStatusComp.Get()->GetResultAttackDuration()+1.0f; 
		float attackSpeed = OwnerStatusComp.Get()->GetResultAttackSpeed() / 100.0f;
		// (데미지, 지속시간, 크기, 공격속도)
		NewTrail->InitializeTrail(FinalDamage, Duration, Size, attackSpeed);
	}
}

