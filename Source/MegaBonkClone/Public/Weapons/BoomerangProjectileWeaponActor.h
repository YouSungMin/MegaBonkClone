// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ProjectileWeaponActor.h"
#include "BoomerangProjectileWeaponActor.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API ABoomerangProjectileWeaponActor : public AProjectileWeaponActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:
	// 충돌 감지 함수
	//UFUNCTION() //부모에서 이미 UFUNCTION이므로 생략
	virtual void OnProjectileHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	//ObjectPool 인터페이스 구현부
	virtual void OnPoolActivate_Implementation() override;

	virtual void OnPoolDeactivate_Implementation() override;

	void StartReturn(); // 돌아오기 시작하는 함수

public:
	// 몇 초 뒤에 돌아올지 (데이터 테이블의 Duration 등을 활용 가능)
	float ReturnDelay = 2.0f;

	// 돌아올 때 가속도 (클수록 급격하게 꺾임)
	float HomingAccel = 3000.0f;

private:
	bool bIsReturning = false; // 현재 돌아오는 중인지

	TObjectPtr<APawn> OwnerPawn = nullptr;
};
