// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/WeaponDataStructs.h"
#include "Data/ItemDataStructs.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemUpdate, FName, ItemID, const FItemData&, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

public:
	// 데이터 테이블의 RowName (Key)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	// 현재 레벨 (비전서용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 1;

	// 보유 개수 (일반 아이템용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 1;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEGABONKCLONE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
public:	
	// 비전서 획득 
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddSecretBook(FName BookID, float AddValue);

	// 일반 아이템 획득
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FName ItemID, int32 Count = 1);

	// 일반 아이템 제거 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(FName ItemID, int32 Count = 1);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	const TArray<FInventorySlot>& GetSecretBookSlots() const { return SecretBookSlots; }

	// ID를 통해 아이템 상세 데이터를 찾아오는 함수 
	FItemData* GetItemInfo(FName ItemID) const;

	UPROPERTY(BlueprintAssignable)
	FOnItemUpdate OnItemAdd;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInventoryUpdated OnInventoryUpdated;

	void ProcessProcTrigger(EProcTriggerType Trigger, AActor* TargetActor = nullptr, float TriggerValue = 0.0f);

	// 일반 아이템 슬롯 정보 반환
	UFUNCTION(BlueprintPure, Category = "Inventory|Items")
	const TArray<FInventorySlot>& GetGeneralItemSlots() const { return GeneralItems; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void ApplyPassiveStats(const FItemData& ItemData, int32 Count, bool bIsFirstGet, float AddValue, EItemType ItemType);
	void RemovePassiveStats(const FItemData& ItemData, int32 Count, bool bIsFirstGet, float AddValue, EItemType ItemType);
private:
	// 내부적으로 효과를 실행하는 분기 함수
	void ExecuteProcEffect(const FItemProcData& Proc, int32 StackCount, AActor* TargetActor, float TriggerValue);

	// 쿨타임 체크 헬퍼
	bool IsOnCooldown(FName ItemID, int32 ProcIndex);
	void SetCooldown(FName ItemID, int32 ProcIndex, float CooldownTime);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable")
	TObjectPtr<UDataTable> ItemDataTable = nullptr;

	// 2. 비전서 슬롯 (최대 3개)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|SecretBooks")
	TArray<FInventorySlot> SecretBookSlots;

	// 3. 일반 아이템 (무한)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Items")
	TArray<FInventorySlot> GeneralItems;

	// 쿨타임 관리용 맵
	UPROPERTY()
	TMap<FName, double> ProcCooldownMap;
private:
	const int32 MaxSecretBookCount = 3;

	TObjectPtr<class UStatusComponent> CachedStatusComponent = nullptr;

	void ProcessPassiveStats(const FItemData& ItemData, int32 Count, bool bIsFirstGet, float AddValue, EItemType ItemType, float Multiplier);
};
