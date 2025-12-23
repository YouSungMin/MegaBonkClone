// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ItemDataStructs.h"
#include "SecretBookSlotWidget.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API USecretBookSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//위젯데이터 세팅
	void InitializeSecretBookSlot(FName InItemID, int32 InLevel, UInventoryComponent* InInventory);
	//설정된 데이터 기반으로 위젯내용 갱신
	void RefreshSecretBookSlot();
	

protected:
	void ClearSecretBookSlot();


protected:
	UPROPERTY(BlueprintReadOnly, Category = "UI|Inventory", meta = (BindWidget))
	TObjectPtr<class UImage> SecretIconImage;

	UPROPERTY(BlueprintReadOnly, Category = "UI|Inventory", meta = (BindWidget))
	TObjectPtr<class UTextBlock> LevelText;

private:

	FName ItemID = NAME_None;
	int32 Level = 1;

	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> TargetSecretBookInventory;
};
