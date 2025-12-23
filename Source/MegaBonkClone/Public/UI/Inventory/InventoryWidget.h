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

	//�ܺ� �κ��丮 ������Ʈ ����
	void InitializeInventoryWidget(class UInventoryComponent* InventoryComponent);
	
	//���� �κ��丮�� UIä���
	UFUNCTION()
	void RefreshInventoryWidget();

	UFUNCTION()
	void ClearInventoryWidget();

	UPROPERTY(BlueprintAssignable, Category = "UI|Inventory")
	FOnInventoryCloseRequested OnInventoryCloseRequested;

private:
	//������ �߰��� �̺�Ʈ ��ü ��������
	UFUNCTION()
	void HandleItemAdded(FName ItemID, const FItemData& ItemData);

	//�ݱ��ư����
	/*UFUNCTION()
	void OnCloseInventoryClicked();*/

protected:
	//UPROPERTY(meta = (BindWIdget))
	//TObjectPtr<class UButton> CloseButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI|Inventory|Items")
	TObjectPtr<class UUniformGridPanel> ItemSlotGridPanel = nullptr;

private:
	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> TargetInventory = nullptr;

	//NativeConstruct���� �׸����г� 8�� ���� ĳ��
	UPROPERTY()
	TArray<TObjectPtr<class UItemSlotWidget>> ItemSlotWidgets;

};
