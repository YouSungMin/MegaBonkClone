// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameItemBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UInGameItemBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	//외부 인벤토리 컴포넌트 주입
	void InitializeInventoryWidget(class UInventoryComponent* InventoryComponent);

	//현재 인벤토리로 UI채우기
	UFUNCTION()
	void RefreshInventoryWidget();


	UFUNCTION()
	void ClearInventoryWidget();

private:
	//아이템 추가시 이벤트 전체 리프레시
	UFUNCTION()
	void HandleItemAdded(FName ItemID, const FItemData& ItemData);


protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI|Inventory|Items")
	TObjectPtr<class UWrapBox> ItemWrapBox = nullptr;

private:
	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> TargetInventory = nullptr;

	//NativeConstruct에서 그리드패널 8개 슬롯 캐싱
	UPROPERTY()
	TArray<TObjectPtr<class UItemSlotWidget>> ItemSlotWidgets;

};
