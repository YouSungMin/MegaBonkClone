// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/TypeEnums.h"
#include "RewardSystemComponent.generated.h"

// UI에 전달할 데이터 구조체
USTRUCT(BlueprintType)
struct FRewardOption
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(BlueprintReadOnly)
	EItemType Type;

	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(BlueprintReadOnly)
	FText Name;

	UPROPERTY(BlueprintReadOnly)
	EItemStatType StatType;

	UPROPERTY(BlueprintReadOnly)
	FText Description;

	UPROPERTY(BlueprintReadOnly)
	int32 NewLevel = 1;

	UPROPERTY(BlueprintReadOnly)
	float CurrentStatValue = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float NextStatValue = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	EItemGrade Rarity = EItemGrade::Common;
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEGABONKCLONE_API URewardSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URewardSystemComponent();


	// 레벨업 시 호출: 보상 생성 -> UI 오픈 -> 일시정지
	UFUNCTION(BlueprintCallable)
	void GenerateLevelUpRewards();

	// UI에서 카드 선택 시 호출
	UFUNCTION(BlueprintCallable)
	void SelectReward(const FRewardOption& SelectedOption);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// BP에서 위젯 생성을 위한 이벤트
	UFUNCTION(BlueprintImplementableEvent, Category = "Reward")
	void OnRewardsGenerated(const TArray<FRewardOption>& Options);

private:
	// [추가] 무기 강화 로직을 처리하는 내부 함수
	void ApplyWeaponUpgradeLogic(const struct FWeaponData* Row, int32 CurLv, bool bHas, struct FRewardOption& OutOption);
	
	// 랜덤 등급 계산
	EItemGrade CalculateRandomRarity();

protected:
	// 무기데이터 테이블
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<class UDataTable> WeaponDataTable;

	// 아이템 데이터 테이블
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<class UDataTable> ItemDataTable;

	UPROPERTY(EditAnywhere, Category = "Config")
	TMap<EItemGrade, float> RarityWeights;

	UPROPERTY()
	TObjectPtr<class UWeaponSystemComponent> WeaponComp;

	// 등급별 스탯 증가 배율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TMap<EItemGrade, float> GradeMultipliers;
	UPROPERTY()
	TObjectPtr<class UInventoryComponent> InventoryComp;
		
};
