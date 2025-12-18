// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BouncingProjectileWeaponActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"

void ABouncingProjectileWeaponActor::InitializeBouncing(int32 InBounces, float InBounceRange)
{
	BouncesLeft = InBounces;
	BounceRange = InBounceRange;
	HitHistory.Empty();

	bPenetrate = true;
}

void ABouncingProjectileWeaponActor::OnPoolActivate_Implementation()
{
	Super::OnPoolActivate_Implementation();

	HitHistory.Empty();
}

void ABouncingProjectileWeaponActor::OnPoolDeactivate_Implementation()
{
	Super::OnPoolDeactivate_Implementation();

	HitHistory.Empty();
}

void ABouncingProjectileWeaponActor::OnProjectileHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 1. 적 유효성 검사
	if (!OtherActor || !OtherActor->ActorHasTag("Enemy")) return;

	// 중복 피격 방지 (이미 맞은 놈이면 패스)
	if (HitHistory.Contains(OtherActor)) return;

	Super::OnProjectileHit(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// 맞은 놈 기록
	HitHistory.Add(OtherActor);

	// 3. 바운스 로직
	if (BouncesLeft > 0)
	{
		// 다음 타겟 찾기
		AActor* NextTarget = FindNextTarget(OtherActor->GetActorLocation());

		if (NextTarget)
		{
			BouncesLeft--; // 횟수 차감

			// 방향 전환 (현재 위치 -> 다음 적 위치)
			FVector Direction = (NextTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();

			// 속도 벡터 갱신
			if (MovementComp)
			{
				MovementComp->Velocity = Direction * MovementComp->MaxSpeed;
			}

			// (선택) 회전도 맞춰주기
			SetActorRotation(Direction.Rotation());

			return; // Destroy 되지 않고 날아감
		}
	}

	// 더 이상 튕길 곳이 없거나 횟수를 다 썼으면 삭제
	Destroy();
}

AActor* ABouncingProjectileWeaponActor::FindNextTarget(const FVector& Origin)
{
	// 오버랩으로 주변 적 검색
	TArray<FOverlapResult> Overlaps;
	FCollisionShape Shape;
	Shape.SetSphere(BounceRange);

	FCollisionObjectQueryParams Params;
	Params.AddObjectTypesToQuery(ECC_Pawn);
	Params.AddObjectTypesToQuery(ECC_WorldDynamic); // 적이 Pawn이라 가정

	GetWorld()->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, Params, Shape);

	AActor* BestTarget = nullptr;
	float MinDistSq = FLT_MAX;

	for (const FOverlapResult& Result : Overlaps)
	{
		AActor* Candidate = Result.GetActor();
		// 1. 적 태그 확인
		// 2. 이미 맞춘 놈(HitHistory) 제외
		// 3. 나 자신 제외
		if (Candidate && Candidate->ActorHasTag("Enemy") && !HitHistory.Contains(Candidate))
		{
			float DistSq = FVector::DistSquared(Origin, Candidate->GetActorLocation());
			if (DistSq < MinDistSq)
			{
				MinDistSq = DistSq;
				BestTarget = Candidate;
			}
		}
	}

	return BestTarget;
}
