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

	void InitializeInventoryWidget(class UInventoryComponent* InventoryComponent);
	void RefreshInventoryWidget();
	void ClearInventoryWidget();

	UPROPERTY(BlueprintAssignable, Category = "UI|Inventory")
	FOnInventoryCloseRequested OnInventoryCloseRequested;

private:
	UFUNCTION()
	void OnCloseInventoryClicked();

protected:
	//UPROPERTY(meta = (BindWIdget))
	//TObjectPtr<class UButton> CloseButton = nullptr;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> ItemSlotGridPanel = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UItemSlotWidget> DetailInfoPanel = nullptr;

private:
	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> TargetInventory = nullptr;


	TArray<TObjectPtr<class UItemSlotWidget>> ItemSlotWidgets;

};
