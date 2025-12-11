// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileWeaponBaseActor.generated.h"

UCLASS()
class MEGABONKCLONE_API AProjectileWeaponBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileWeaponBaseActor();

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
	
	
};
