// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShadyStoreSlotWidget.generated.h"

class UTextBlock;
class UImage;
class UButton;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShadySlotClicked, int32, SlotIndex);

UCLASS()
class MEGABONKCLONE_API UShadyStoreSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	// 슬롯 UI에 정보 세팅 + 이 슬롯이 몇번째인지 저장
	UFUNCTION(BlueprintCallable)
	void SetSlotInfo(const FShopSlotInfo& Info, int32 InSlotIndex);

	UPROPERTY(BlueprintAssignable)
	FOnShadySlotClicked OnSlotClicked;

private:
	UFUNCTION()
	void HandleClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Type = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Title = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Description = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SlotButton = nullptr;

private:
	int32 CachedIndex = INDEX_NONE;
};
