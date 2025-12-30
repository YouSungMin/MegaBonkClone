// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryOwner.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryOwner : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MEGABONKCLONE_API IInventoryOwner
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 아이템 추가 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	void ReceiveItem(FName ItemID, int32 Count);

	// 상자를 열때 필요한 골드 체크 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status")
	float GetAdjustedCost(float BaseCost) ;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status")
	// 골드를 사용하는 함수
	bool UseGold(float Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status")
	// 현재 골드를 리턴하는 함수
	float GetGold(float Amount);

	// 상자를 연 횟수를 증가시키는 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status")
	void NotifyChestOpened();
};
