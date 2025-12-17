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

public:

protected:

	//발사할 발사체
	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeaponBase")
	TSubclassOf<class AProjectileWeaponActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeaponBase")
	bool bOnlyFireWhenTargetFound = false;


};
