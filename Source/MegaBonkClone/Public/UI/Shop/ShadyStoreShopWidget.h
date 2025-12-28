// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShadyStoreShopWidget.generated.h"

class UShadyStoreSlotWidget;
class UButton;
class AShadyGuyActor;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UShadyStoreShopWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void InitWithShadyGuy(class AShadyGuyActor* InGuy, const TArray<FShopSlotInfo>& Items);

private:
	// 슬롯 선택됨
	UFUNCTION()
	void OnSlotPicked(int32 SlotIndex);

	// 상점 나가기
	UFUNCTION()
	void OnExitClicked();

private:
	// 상점 주인 참조
	UPROPERTY()
	TObjectPtr<AShadyGuyActor> GuyRef = nullptr;

	// 캐시된 아이템들
	UPROPERTY()
	TArray<FShopSlotInfo> CachedItems;
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UShadyStoreSlotWidget> Slot1 = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UShadyStoreSlotWidget> Slot2 = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UShadyStoreSlotWidget> Slot3 = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton = nullptr;
	
};
