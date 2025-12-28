// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MicroSlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMicroSlotClicked, FName, ItemID);

class UImage;
class UButton;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UMicroSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


	// 슬롯에 표시할 데이터 세팅
	UFUNCTION(BlueprintCallable)
	void SetSlotInfo(const FMicrowaveSlotInfo& Info);

	UPROPERTY(BlueprintAssignable)
	FOnMicroSlotClicked OnSlotClicked;

private:
	UFUNCTION()
	void HandleClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SlotButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image = nullptr;	

private:
	FName CachedItemID = NAME_None;
};
