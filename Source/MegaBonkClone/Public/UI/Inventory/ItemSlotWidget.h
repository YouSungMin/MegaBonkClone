// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//위젯데이터 세팅
	//void InitializeItemSlot(int32 Int32 , struct int32 InSlotData);

	//설정된 데이터 기반으로 위젯내용 갱신
	void RefreshItemSlotWidget();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "UI|Inventory", meta = (BindWidget))
	TObjectPtr<class UImage> ItemIconImage;

	UPROPERTY(BlueprintReadOnly, Category = "UI|Inventory", meta = (BindWidget))
	TObjectPtr<class UTextBlock> LevelText;

};