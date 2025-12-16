// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ProjectileWeaponActor.h"
#include "BouncingProjectileWeaponActor.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API ABouncingProjectileWeaponActor : public AProjectileWeaponActor
{
	GENERATED_BODY()
	
public:
	// 초기화 함수 확장 (바운스 횟수, 탐색 범위 추가)
	void InitializeBouncing(int32 InBounces, float InBounceRange);

	//ObjectPool 인터페이스 구현부
	virtual void OnPoolActivate_Implementation() override;

	virtual void OnPoolDeactivate_Implementation() override;

protected:
	virtual void OnProjectileHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// 다음 타겟 찾는 헬퍼 함수
	AActor* FindNextTarget(const FVector& Origin);

private:
	int32 BouncesLeft = 5;   // 남은 튕김 횟수
	float BounceRange = 1000.0f; // 튕길 때 다음 적 찾는 범위

	// 이미 맞춘 적은 다시 안 맞추기 위해 기록
	UPROPERTY()
	TArray<AActor*> HitHistory;
};
