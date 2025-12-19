// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/WeaponDataStructs.h"
#include "Data/ItemDataStructs.h"
#include "InventoryComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemUpdate, FName, ItemID, const FItemData&, ItemData);
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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// 비전서 획득 
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddSecretBook(FName BookID);

	// 일반 아이템 획득
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FName ItemID, int32 Count = 1);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	const TArray<FInventorySlot>& GetSecretBookSlots() const { return SecretBookSlots; }

	// ID를 통해 아이템 상세 데이터를 찾아오는 함수 
	FItemData* GetItemInfo(FName ItemID) const;

	UPROPERTY(BlueprintAssignable)
	FOnItemUpdate OnItemAdd;
protected:
	void ApplyPassiveStats(const FItemData& ItemData, int32 Count, bool bIsFirstGet);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable")
	TObjectPtr<UDataTable> ItemDataTable = nullptr;

	// 2. 비전서 슬롯 (최대 3개)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|SecretBooks")
	TArray<FInventorySlot> SecretBookSlots;

	// 3. 일반 아이템 (무한)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Items")
	TArray<FInventorySlot> GeneralItems;
private:
	const int32 MaxSecretBookCount = 3;

	TObjectPtr<class UStatusComponent> CachedStatusComponent = nullptr;
};
