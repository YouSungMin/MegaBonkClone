// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrailWeaponActor.generated.h"

UCLASS()
class MEGABONKCLONE_API ATrailWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrailWeaponActor();

	//장판의 능력치 (데미지 , 지속시간 , 크기 , 공격속도)
	void InitializeTrail(float InDamage, float InDuration, float InScale, float AttackSpeed);

	// 매 틱(Interval)마다 장판 위의 적에게 데미지
	UFUNCTION()
	void OnDamageTick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Trail|Component")
	TObjectPtr<USceneComponent> Root = nullptr;

	// 적이 밟았는지 감지할 영역
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UCapsuleComponent* OverlapComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UNiagaraComponent* EffectComp;

private:
	float Damage = 0.0f;
	FTimerHandle DamageTimerHandle;
};
