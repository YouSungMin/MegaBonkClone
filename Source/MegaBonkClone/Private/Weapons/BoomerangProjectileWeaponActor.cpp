// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BoomerangProjectileWeaponActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void ABoomerangProjectileWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	

	//타이머 설정: ReturnDelay 후에 StartReturn 호출
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &ABoomerangProjectileWeaponActor::StartReturn, ReturnDelay, false);

	OwnerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
}

void ABoomerangProjectileWeaponActor::OnProjectileHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 1. 돌아오는 중에 주인(플레이어)과 닿았다면? -> 회수(Destroy)
	if (bIsReturning && OtherActor == GetOwner())
	{
		Destroy();
		return;
	}

	// 2. 적과 닿았을 때 (부모 로직 사용: 데미지 주기)
	Super::OnProjectileHit(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	bPenetrate = true; // 관통여부 설정 //타이밍상 이쯤
}

void ABoomerangProjectileWeaponActor::StartReturn()
{
	if (OwnerPawn)
	{
		bIsReturning = true;

		// 유도탄 설정 켜기
		MovementComp->bIsHomingProjectile = true;
		MovementComp->HomingTargetComponent = OwnerPawn->GetRootComponent();
		MovementComp->HomingAccelerationMagnitude = HomingAccel;

		// 돌아올 때 속도 제한 해제 (더 빨리 오게)
		MovementComp->MaxSpeed = MovementComp->InitialSpeed * 1.5f;
	}
	else
	{
		Destroy(); // 주인 없으면 자폭
	}
}
