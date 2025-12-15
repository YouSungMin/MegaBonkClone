// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/WeaponBase.h"
#include "TrailWeaponBase.generated.h"

UCLASS()
class MEGABONKCLONE_API ATrailWeaponBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrailWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:


protected:

	// 타이머가 공격 신호를 보낼 때 실행됨 (WeaponBase의 StartAttackTimer -> InvokeAttack -> 이 함수)
	virtual void AttackWeapon_Implementation() override;

	// 바닥에 깔 장판 클래스 (BP에서 지정)
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class ATrailWeaponActor> TrailClass = nullptr;

};
