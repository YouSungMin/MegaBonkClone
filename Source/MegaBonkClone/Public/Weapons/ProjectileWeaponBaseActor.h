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

	void SetHomingTarget(AActor* Target);
	//오브젝트에 오버랩 되었을때 실행할 함수
	virtual void OnBeginWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor) override;
	//오브젝트에 오버랩 끝났을때 실행할 함수
	virtual void OnEndWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor) override;
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
	TObjectPtr<class URotatingMovementComponent> RotatingComponent = nullptr;
	
	
};
