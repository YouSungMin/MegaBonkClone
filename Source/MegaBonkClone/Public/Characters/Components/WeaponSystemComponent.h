// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/WeaponDataStructs.h"
#include "WeaponSystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponChanged);

USTRUCT(BlueprintType)
struct FWeaponSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 1;

	// 실제 스폰된 무기 액터 (직접 접근 가능!)
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AWeaponBase> WeaponInstance = nullptr;

	// 슬롯이 비었는지 확인하는 헬퍼
	bool IsEmpty() const { return WeaponID.IsNone(); }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEGABONKCLONE_API UWeaponSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponSystemComponent();

	// =========================================================
	// [기능 함수]
	// =========================================================

	// 무기 신규 추가 (스폰 + 슬롯 등록)
	UFUNCTION(BlueprintCallable)
	void AddWeapon(FName WeaponID);

	// 무기 레벨업 (기존 무기 찾아서 스탯 강화)
	UFUNCTION(BlueprintCallable)
	void LevelUpWeapon(FName WeaponID, EItemStatType StatType, float IncreaseValue);

	// 특정 무기 보유 여부 확인 (RewardSystem 연동용)
	UFUNCTION(BlueprintPure)
	bool HasWeapon(FName WeaponID) const;

	// 무기 레벨 가져오기
	UFUNCTION(BlueprintPure)
	int32 GetWeaponLevel(FName WeaponID) const;

	// 현재 무기 개수 (빈 슬롯 제외)
	UFUNCTION(BlueprintPure)
	int32 GetCurrentWeaponCount() const;
	
	FWeaponData* GetWeaponInfo(FName WeaponID) const;

	// UI에서 슬롯 정보를 가져갈 때 사용
	UFUNCTION(BlueprintPure)
	const TArray<FWeaponSlot>& GetWeaponSlots() const { return ActiveSlots; }

	// 클래스 정보로 ID 찾기 (시작 무기 지급용)
	FName ResolveWeaponIDFromClass(TSubclassOf<AActor> InWeaponClass) const;

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void Debug_TestWeapon(FName WeaponID);

public:
	UPROPERTY(BlueprintAssignable)
	FOnWeaponChanged OnWeaponChanged;

protected:
	// =========================================================
	// [데이터 및 설정]
	// =========================================================

	// 무기 데이터 테이블
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> WeaponDataTable;


private:
	// [통합] 실제 관리되는 무기 슬롯 배열 (이거 하나로 끝!)
	UPROPERTY(VisibleInstanceOnly, Category = "State")
	TArray<FWeaponSlot> ActiveSlots;

	// 최대 무기 개수
	int32 MaxWeaponSlots = 3;
};
