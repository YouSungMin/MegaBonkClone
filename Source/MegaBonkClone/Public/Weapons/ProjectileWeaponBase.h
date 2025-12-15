// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/WeaponBase.h"
#include "ProjectileWeaponBase.generated.h"

UCLASS()
class MEGABONKCLONE_API AProjectileWeaponBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileWeaponBase();

	//유도될 대상 세팅
	UFUNCTION(BlueprintCallable)
	void SetHomingTarget(AActor* Target);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;





protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponBase|Mesh")
	TObjectPtr<UStaticMeshComponent> BaseMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponBase|Collision")
	TObjectPtr<class UCapsuleComponent> Collision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponBase|Projectile")
	TObjectPtr<class UProjectileMovementComponent> ProjectileComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponBase|Rotator")
	TObjectPtr<class URotatingMovementComponent> RotatingComponent=nullptr;
	// --- 설정 변수 ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Banana Stats")
	float ReturnDelay = 0.5f; // 몇 초 뒤에 돌아올지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Banana Stats")
	float HomingAccel = 3000.0f; // 돌아올 때의 가속도 (유도 성능)

private:
	float LifeSpan = 0.0f;

	FTimerHandle ReturnTimerHandle; // 타이머 핸들

	bool bIsReturning = false;

	TObjectPtr<APawn> OwnerPawn = nullptr;

	// 타이머가 끝나면 호출될 함수
	void StartReturn();
};
