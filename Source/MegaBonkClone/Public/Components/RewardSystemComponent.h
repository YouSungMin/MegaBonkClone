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

	UPROPERTY(BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(BlueprintReadWrite)
	EItemType Type;

	UPROPERTY(BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(BlueprintReadWrite)
	FText Name;

	UPROPERTY(BlueprintReadWrite)
	EItemStatType StatType;

	UPROPERTY(BlueprintReadWrite)
	FText Description;

	UPROPERTY(BlueprintReadWrite)
	int32 NewLevel = 1;

	UPROPERTY(BlueprintReadWrite)
	float CurrentStatValue = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float NextStatValue = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	EItemGrade Rarity = EItemGrade::Common;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRewardsGenerated, const TArray<FRewardOption>&, RewardOptions);

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

	// 델리게이트 인스턴스 정의
	UPROPERTY(BlueprintAssignable, Category = "Reward")
	FOnRewardsGenerated OnRewardsGenerated;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// [추가] 무기 강화 로직을 처리하는 내부 함수
	void ApplyWeaponUpgradeLogic(const struct FWeaponData* Row, int32 CurLv, bool bHas, struct FRewardOption& OutOption);
	
	void ApplySecretBookLogic(const struct FItemData* Row, int32 CurLv, bool bHas, struct FRewardOption& OutOption);
	// 랜덤 등급 계산
	EItemGrade CalculateRandomRarity();

	// 델리게이트 시그니처(float)와 맞추기 위한 래퍼 함수
	UFUNCTION()
	void OnPlayerLevelChanged(float NewLevel);

	// 컴포넌트들을 찾아서 캐싱하는 함수
	bool FindComponents();
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

	UPROPERTY()
	TObjectPtr<class UInventoryComponent> InventoryComp;

	UPROPERTY()
	TObjectPtr<class UStatusComponent> StatusComp;


	// 등급별 스탯 증가 배율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TMap<EItemGrade, float> GradeMultipliers;
		
};
