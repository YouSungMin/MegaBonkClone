// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/WeaponDataStructs.h"
#include "Data/ItemDataStructs.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

public:
	// 데이터 테이블의 RowName (Key)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	// 현재 레벨 (무기/비전서용)
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
	// 무기 획득
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddWeapon(FName WeaponID);

	// 비전서 획득 
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddSecretBook(FName BookID);

	// 일반 아이템 획득
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FName ItemID, int32 Count = 1);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable")
	TObjectPtr<UDataTable> WeaponDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable")
	TObjectPtr<UDataTable> ItemDataTable;

	// 1. 무기 슬롯 (최대 3개)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Weapons")
	TArray<FInventorySlot> WeaponSlots;

	// 2. 비전서 슬롯 (최대 3개)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|SecretBooks")
	TArray<FInventorySlot> SecretBookSlots;

	// 3. 일반 아이템 (무한)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Items")
	TArray<FInventorySlot> GeneralItems;
private:
	const int32 MaxWeaponCount = 3;
	const int32 MaxSecretBookCount = 3;
};
