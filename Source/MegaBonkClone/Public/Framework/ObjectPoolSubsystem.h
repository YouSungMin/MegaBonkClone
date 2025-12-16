// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolSubsystem.generated.h"

struct FActorSpawnParameters;

USTRUCT()
struct FObjectPoolQueue
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<AActor>> Pool;
};
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	//템플릿 인수로 캐스트 하여 SpawnPooledActor 실행하는 함수
	template<typename T>
	T* SpawnPoolActor(UClass* Class, FVector location, FRotator rotation, AActor* Owner, APawn* Instigator)
	{
		return Cast<T>(SpawnPooledActor(Class, location, rotation, Owner, Instigator));
	}

	//오브젝트 풀을 이용한 스폰함수
	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
	AActor* SpawnPooledActor(UClass* Class, FVector location, FRotator rotation, AActor* Owner, APawn* Instigator);

	//오브젝트 풀에 되돌리는 함수
	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
	void ReturnToPool(AActor* InActor);
	
private:
	UPROPERTY()
	TMap<UClass*, FObjectPoolQueue> PoolMap;
};
