// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/SanctuaryBase.h"
#include "Data/TypeEnums.h"
#include "RechargeSanctuary.generated.h"

// UI에 전달할 단일 보상 정보 구조체
USTRUCT(BlueprintType)
struct FSanctuaryRewardInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	EItemStatType StatType;

	UPROPERTY(BlueprintReadOnly)
	EItemGrade Rarity;

	// 실제 적용될 수치
	UPROPERTY(BlueprintReadOnly)
	float Value = 0.0f;
};
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSanctuaryRewardsGenerated, const TArray<FSanctuaryRewardInfo>&, Rewards, ARechargeSanctuary*, SanctuaryActor);
UCLASS()
class MEGABONKCLONE_API ARechargeSanctuary : public ASanctuaryBase
{
	GENERATED_BODY()
public:
	ARechargeSanctuary();

	UPROPERTY(BlueprintAssignable, Category = "Sanctuary|Event")
	FOnSanctuaryRewardsGenerated OnRewardsGenerated;
protected:
	virtual void BeginPlay()override;

	virtual void ApplyEffect_Implementation(AActor* Player)override;

	UFUNCTION(BlueprintCallable, Category = "Sanctuary")
	void ApplySelectedReward(const FSanctuaryRewardInfo& RewardInfo);

	UFUNCTION()
	void OnProximityBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnProximityEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 타이머가 끝났을 때 실행
	void OnChargeComplete();
private:
	EItemGrade PickRandomRarity();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> ProximitySphere;

	// 등장 가능한 스탯 목록 (에디터에서 원하는 스탯만 골라서 넣어주세요)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sanctuary|Config")
	TArray<EItemStatType> AvailableStats;

	// 2. 스탯별 기본값 (Key: 스탯, Value: 커먼 등급 기준 수치)
	// 예: Damage: 10.0, CritChance: 5.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sanctuary|Config")
	TMap<EItemStatType, float> StatBaseValues;

	// 등급별 가중치 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sanctuary|Config")
	TMap<EItemGrade, float> RarityWeights;

	// 4. [추가] 등급별 스탯 배율 (Common=1.0 기준)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sanctuary|Config")
	TMap<EItemGrade, float> RarityMultipliers;

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
	TWeakObjectPtr<AActor> OverlappingPlayer;
};
