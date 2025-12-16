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

void ABoomerangProjectileWeaponActor::OnPoolActivate_Implementation()
{
	Super::OnPoolActivate_Implementation();

	bIsReturning = false;

	// 3. 무브먼트 컴포넌트 원상복구 (유도 기능 끄기)
	if (MovementComp)
	{
		MovementComp->bIsHomingProjectile = false;
		MovementComp->HomingTargetComponent = nullptr;
		// MaxSpeed는 InitializeProjectile에서 초기화되므로 놔둬도 되지만, 
		// 확실하게 하려면 여기서 InitialSpeed로 맞춰도 됩니다.
	}

	// 4. 주인 다시 찾기 (안전장치)
	if (!OwnerPawn)
	{
		OwnerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}

	// 5. [중요] 돌아오는 타이머 다시 걸기 (BeginPlay에 있던 코드 이동)
	// ReturnDelay 시간이 지나면 StartReturn 함수가 호출되도록 예약
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &ABoomerangProjectileWeaponActor::StartReturn, ReturnDelay, false);
}

void ABoomerangProjectileWeaponActor::OnPoolDeactivate_Implementation()
{
	Super::OnPoolDeactivate_Implementation();

	bIsReturning = false;
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
