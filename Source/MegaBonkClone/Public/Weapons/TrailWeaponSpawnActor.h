// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrailWeaponSpawnActor.generated.h"

UCLASS()
class MEGABONKCLONE_API ATrailWeaponSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrailWeaponSpawnActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// 2. 타이머 핸들 선언
	FTimerHandle SpawnTimerHandle;

	// 3. 1초마다 호출될 함수 선언
	void SpawnTrailWeapon();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrailWeapon|Component")
	TObjectPtr<USceneComponent> BaseRoot = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrailWeapon")
	TSubclassOf<class ATrailWeaponActor> TrailClass = nullptr;
};
