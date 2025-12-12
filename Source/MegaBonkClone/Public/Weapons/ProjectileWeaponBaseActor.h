// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/WeaponBase.h"
#include "ProjectileWeaponBaseActor.generated.h"

UCLASS()
class MEGABONKCLONE_API AProjectileWeaponBaseActor : public AWeaponBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileWeaponBaseActor();
	
	void LaunchProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectileWeapon")
	TSubclassOf<class AProjectileBase> ProjectileClass = nullptr;

private:
	FTimerHandle ProjectileTimerHandle;

	float ProjectileTimerTime = 1.0f;
};
