// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ObjectPoolInterface.h"
#include "Interfaces/PickupInterface.h"
#include "PickupBaseActor.generated.h"

UCLASS()
class MEGABONKCLONE_API APickupBaseActor : public AActor , public IPickupInterface, public IObjectPoolInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBaseActor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CheckOverlappingAndPickup();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// IPickupable의 구현(먹기 시작할 때 실행되는 함수)
	virtual void OnPickup_Implementation(AActor* Target) override;

	// IPickupable의 구현(먹기가 완료되었을 때 실행되는 함수)
	virtual void OnPickupComplete_Implementation() override;

	inline bool IsPickup() const { return bPickuped;}

	virtual void OnPoolActivate_Implementation() override;
	virtual void OnPoolDeactivate_Implementation() override;

	void EnablePickup();
private:
	UFUNCTION()
	void OnTimelineUpdate(float Value);

	UFUNCTION()
	void OnTimelineFinished();
protected:
	// 물리 적용용 루트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> BaseRoot = nullptr;

	// 아이템 외형
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

	// 타임라인 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UTimelineComponent> PickupTimeline = nullptr;

	// 스폰 후에 먹을 수 있기 될 때까지의 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float PickupableTime = 0.5f;

	// 아이템 회전 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RotateSpeed = 180.0f;

	// 픽업 획득 효과용 거리 보간 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UCurveFloat> DistanceCurve = nullptr;

	//// 픽업 획득 효과용 높이 커브
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	//TObjectPtr<UCurveFloat> HeightCurve = nullptr;

	// 픽업 획득 효과용 스케일 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UCurveFloat> ScaleCurve = nullptr;

	// 아이템 획득에 걸리는 시간(초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float Duration = 0.5f;

	// 아이템 획득시 튀어 오르는 높이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float PickupHeight = 50.0f;

	// 이 픽업 아이템을 먹은 액터
	UPROPERTY()
	TWeakObjectPtr<AActor> PickupOwner = nullptr;

	// true 시 플레이어 카메라를 바라봄
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	bool bIsBillboard = false;

	FRotator InitialMeshRotation = FRotator(0,0,0);

	FTimerHandle CheckTimerHandle;

	FTimerHandle EnablePickupTimerHandle;
private:
	// 획득했을 때 메시 위치(월드)
	FVector PickupStartLocation = FVector(0,0,0);

	// 획득 되었는지 여부(true면 획득 처리 중)
	bool bPickuped = false;

	//바로 먹어지는거 검사
	bool bCanBePickedUp = false;

	
};
