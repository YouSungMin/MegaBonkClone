// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/WeaponBase.h"
#include "ProjectileWeaponSpawnActor.generated.h"

UCLASS()
class MEGABONKCLONE_API AProjectileWeaponSpawnActor : public AWeaponBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileWeaponSpawnActor();
	
	//투사체 클래스 발사
	void LaunchProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectileWeapon")
	TSubclassOf<class AProjectileWeaponBase> ProjectileClass = nullptr;

private:
	float MaxWeaponCount = 0.0f;

	FTimerHandle ProjectileTimerHandle;

	float ProjectileTimerTime = 1.0f;
};
