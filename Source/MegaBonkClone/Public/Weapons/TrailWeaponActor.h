// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/WeaponBase.h"
#include "TrailWeaponActor.generated.h"

UCLASS()
class MEGABONKCLONE_API ATrailWeaponActor : public AWeaponBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrailWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Aura|Component")
	TObjectPtr<class UCapsuleComponent> Collision = nullptr;
};
