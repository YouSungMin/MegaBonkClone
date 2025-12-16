// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ObjectPoolInterface.h"
#include "ProjectileWeaponActor.generated.h"

UCLASS()
class MEGABONKCLONE_API AProjectileWeaponActor : public AActor ,public IObjectPoolInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileWeaponActor();

	//투사체 스탯 초기화 함수 (데미지 , 발사체속도, 범위(필요한가),넉백 ,관통형인가 아닌가)
	void InitializeProjectile(float InDamage, float InSpeed, float InRange, float InKnockback, bool bIsPenetrate = false);


	//ObjectPool 인터페이스 구현부
	virtual void OnPoolActivate_Implementation() override;

	virtual void OnPoolDeactivate_Implementation() override;

	//발사체 컴포넌트에 있는 함수
	virtual void LifeSpanExpired() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 충돌 감지 함수
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	//메쉬
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UStaticMeshComponent>  MeshComp = nullptr;

	//콜리전
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UCapsuleComponent> CollisionComp = nullptr; // 혹은 USphereComponent

	//발사체 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UProjectileMovementComponent> MovementComp = nullptr;

	//회전 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class URotatingMovementComponent> RotatingComp =nullptr;

	float Damage = 0.0f;
	float Knockback = 0.0f;
	bool bPenetrate = true; // 관통 여부

	
};
