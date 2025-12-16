// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ObjectPoolInterface.h"
#include "Components/TimelineComponent.h"
#include "TrailWeaponActor.generated.h"

UCLASS()
class MEGABONKCLONE_API ATrailWeaponActor : public AActor , public IObjectPoolInterface
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

	//ObjectPool 인터페이스 구현부
	virtual void OnPoolActivate_Implementation() override;

	virtual void OnPoolDeactivate_Implementation() override;

	virtual void LifeSpanExpired() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//타임라인이 실행될 때 호출될 함수
	UFUNCTION()
	void OnShrinkUpdate(float Value);

	//타임라인이 끝났을 때 호출될 함수
	UFUNCTION()
	void OnShrinkFinished();

	// 작아지기 시작하는 함수
	void StartShrink();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Trail|Component")
	TObjectPtr<USceneComponent> Root = nullptr;

	// 적이 밟았는지 감지할 영역
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCapsuleComponent> OverlapComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UPaperSpriteComponent> SpriteComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> MeshComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timeline")
	TObjectPtr<UTimelineComponent> ScaleTimeline;

	// [2] 에디터에서 만든 커브를 넣을 변수
	UPROPERTY(EditDefaultsOnly, Category = "Timeline")
	TObjectPtr<class UCurveFloat> ScaleCurve;

private:
	float Damage = 0.0f;
	FTimerHandle DamageTimerHandle;
	FVector InitialScale; // 원래 크기 저장용
	FOnTimelineFloat UpdateFunctionFloat; // 델리게이트 (연결 고리)
	FOnTimelineEvent FinishedFunctionEvent; // 델리게이트
};
