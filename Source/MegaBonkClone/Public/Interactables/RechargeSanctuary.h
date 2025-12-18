// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/SanctuaryBase.h"
#include "RechargeSanctuary.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API ARechargeSanctuary : public ASanctuaryBase
{
	GENERATED_BODY()
public:
	ARechargeSanctuary();

protected:
	virtual void BeginPlay()override;

	virtual void ApplyEffect_Implementation(AActor* Player)override;

	UFUNCTION()
	void OnProximityBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnProximityEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 타이머가 끝났을 때 실행
	void OnChargeComplete();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> ProximitySphere;

	UPROPERTY(EditAnywhere, Category = "Recharge")
	float ChargeTime = 3.0f;

	// 금색 성소 확률 (0.0 ~ 1.0)
	UPROPERTY(EditAnywhere, Category = "Recharge")
	float GoldChance = 0.1f;

	// 현재 금색 성소인가?
	bool bIsGoldSanctuary = false;

	// 타이머 핸들
	FTimerHandle ChargeTimerHandle;

	// 충전 중인 플레이어
	UPROPERTY()
	AActor* OverlappingPlayer;
};
