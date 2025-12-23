// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryCloseRequested);
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UInventoryWidget : public UUserWidget
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

	UPROPERTY(BlueprintAssignable, Category = "UI|Inventory")
	FOnInventoryCloseRequested OnInventoryCloseRequested;

private:
	//아이템 추가시 이벤트 전체 리프레시
	UFUNCTION()
	void HandleItemAdded(FName ItemID, const FItemData& ItemData);

	UFUNCTION()
	void HandleSecretAdded(FName ItemID, const FItemData& ItemData);


	//닫기버튼사용시
	/*UFUNCTION()
	void OnCloseInventoryClicked();*/

protected:
	//UPROPERTY(meta = (BindWIdget))
	//TObjectPtr<class UButton> CloseButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI|Inventory|Items")
	TObjectPtr<class UUniformGridPanel> ItemSlotGridPanel = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI|Inventory|Items")
	TObjectPtr<class UUniformGridPanel> SecretSlotGridPanel = nullptr;


	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI|Inventory|Weapon")
	TObjectPtr<class UInGameWeaponBarWidget> WeaponBarWidget = nullptr;

private:
	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> TargetInventory = nullptr;

	//NativeConstruct에서 그리드패널 8개 슬롯 캐싱
	UPROPERTY()
	TArray<TObjectPtr<class UItemSlotWidget>> ItemSlotWidgets;


	//NativeConstruct에서 비전시패널 캐싱
	UPROPERTY()
	TArray<TObjectPtr<class UItemSlotWidget>> SecretSlotWidgets;

};
