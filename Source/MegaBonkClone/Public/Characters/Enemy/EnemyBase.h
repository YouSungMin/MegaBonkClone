// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/EnemyDataStructs.h" 
#include "Items/PickupItem/ResourcePickup.h" 
#include "EnemyBase.generated.h"

UCLASS()
class MEGABONKCLONE_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * 데이터 테이블을 읽어 계산된 개수만큼 경험치와 돈을 바닥에 뿌립니다.
	 * @param DataTable : 몬스터 데이터 테이블
	 * @param RowName : 몬스터 ID (예: "Goblin_01")
	 * @param ScatterRadius : 아이템이 퍼지는 범위 (기본 100)
	 */
	UFUNCTION(BlueprintCallable, Category = "Enemy|Drop")
	void SpawnDrops(UDataTable* DataTable, FName RowName, float ScatterRadius = 100.0f);

	//데미지 UI 액터 스폰 함수
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowDamagePopup(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Enemy|VFX")
	void PlayHitFlash();

protected:
	//원래 색으로 되돌리는 함수
	void ResetEnemyHitFlash();

private:

	// 내부적으로 반복해서 스폰하는 헬퍼 함수
	void SpawnSinglePickup(TSubclassOf<AResourcePickup> ItemClass, EResourceType Type, float AmountPerItem, FVector Location);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Drop")
	TSubclassOf<AResourcePickup> ExpItemClass; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Drop")
	TSubclassOf<AResourcePickup> GoldItemClass; 

	//데미지 UI 액터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class ADamageTextActor> DamageTextActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|VFX")
	FName HitFlashParamName = "HitColor"; // 머티리얼 파라미터 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|VFX")
	FLinearColor HitFlashColor = FLinearColor(50.0f, 0.0f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|VFX")
	float FlashDuration = 0.3f; // 반짝이는 시간 (초)

private:
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> MeshMIDs;

	FTimerHandle TimerHandle_HitFlash;
};
