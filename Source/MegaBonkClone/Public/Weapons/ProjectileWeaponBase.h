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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void AttackWeapon_Implementation() override;

	UFUNCTION()
	void FireSingleProjectile();


public:

protected:

	//발사할 발사체
	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeaponBase")
	TSubclassOf<class AProjectileWeaponActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeaponBase")
	bool bOnlyFireWhenTargetFound = false;


	// [추가] 연사(Burst)를 위한 타이머 핸들
	FTimerHandle BurstTimerHandle;

	// [추가] 이번 공격에서 남은 발사체 수
	int32 BurstShotsLeft = 0;

	
};
