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

	// 1. 발사 위치: 플레이어 위치
	FVector SpawnLoc = OwnerCharacter.Get()->GetActorLocation();
	FRotator SpawnRot = OwnerCharacter.Get()->GetActorRotation(); // 기본은 플레이어가 보는 방향

	//가장 가까운 적 찾기
	
	AActor* NearestEnemy = FindNearestEnemy(); 

	if (bOnlyFireWhenTargetFound && NearestEnemy == nullptr)
	{
		return;
	}

	if (NearestEnemy)
	{
		FVector Dir = NearestEnemy->GetActorLocation() - SpawnLoc;
		SpawnRot = Dir.Rotation();
	}
	
	UObjectPoolSubsystem* poolSystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();


	// 2. 투사체 개수만큼 발사 (샷건처럼 여러 발 나가는 경우 처리)
	int32 NumProjectiles = FMath::Max(1, FMath::FloorToInt(FinalProjectileCount));

	for (int32 i = 0; i < NumProjectiles; ++i)
	{
		// 여러 발이면 각도를 약간씩 틀어줌 (예: -10도, 0도, +10도)
		FRotator CurrentRot = SpawnRot;
		if (NumProjectiles > 1)
		{
			float SpreadAngle = 15.0f; // 부채꼴 각도
			float AngleOffset = SpreadAngle * (i - (NumProjectiles - 1) / 2.0f);
			CurrentRot.Yaw += AngleOffset;
		}

		FActorSpawnParameters Params;
		Params.Owner = this;
		Params.Instigator = Cast<APawn>(OwnerCharacter.Get());

		// 3. 스폰
		//AProjectileWeaponActor* NewProj = GetWorld()->SpawnActor<AProjectileWeaponActor>(ProjectileClass, SpawnLoc, CurrentRot, Params);
		AProjectileWeaponActor* NewProj = poolSystem->SpawnPoolActor<AProjectileWeaponActor>(ProjectileClass, SpawnLoc, CurrentRot, this, Cast<APawn>(OwnerCharacter.Get()));


		// 4. 데이터 주입
		if (NewProj)
		{
			//데미지 갱신
			if (Implements<UWeapon>()) {
				IWeapon::Execute_GetDamageWeapon(this);
			}
			UpdateWeaponStats();

			float FinalDmg;

			if (CheckIsCritical()) {
				FinalDmg = WeaponFinalCriticalDamage;
			}
			else {
				FinalDmg = WeaponFinalDamage;
			}
			
			// ProjectileAttackSize, ProjectileSpeed 등 WeaponBase 변수 활용
			float Duration = 5.0f; // 혹은 사거리
			UE_LOG(LogTemp, Warning, TEXT("FinalProjectileSpeed : %.1f"), FinalProjectileSpeed);
			NewProj->InitializeProjectile(
				FinalDmg,
				FinalProjectileSpeed,
				5.0f /*사거리/지속시간*/,
				KnockBack,
				false
			);

			if (FinalAttackSize > 0.0f)
			{
				UE_LOG(LogTemp, Warning, TEXT("FinalAttackSize : %.1f"), FinalAttackSize);
				NewProj->SetActorScale3D(FVector(FinalAttackSize));
			}

			if (ABouncingProjectileWeaponActor* BouncingProj = Cast<ABouncingProjectileWeaponActor>(NewProj))
			{
				BouncingProj->InitializeBouncing(FMath::FloorToInt(FinalReflectCount), 1000.0f);
			}
		}
	}
}





