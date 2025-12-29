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

	//�ܺ� �κ��丮 ������Ʈ ����
	void InitializeInventoryWidget(class UInventoryComponent* InventoryComponent);

	//���� �κ��丮�� UIä���
	UFUNCTION()
	void RefreshInventoryWidget();

	UFUNCTION()
	void ClearInventoryWidget();

	UFUNCTION(BlueprintCallable)
	void ConnectToMicrowave(class AMicrowaveActor* InMicrowave);
private:
	//������ �߰��� �̺�Ʈ ��ü ��������
	UFUNCTION()
	void HandleItemAdded(FName ItemID, const FItemData& ItemData);

	UFUNCTION()
	void HandleInventoryUpdated();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI|Inventory|Items")
	TObjectPtr<class UWrapBox> ItemWrapBox = nullptr;

private:
	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> TargetInventory = nullptr;

	//NativeConstruct���� �׸����г� 8�� ���� ĳ��
	UPROPERTY()
	TArray<TObjectPtr<class UItemSlotWidget>> ItemSlotWidgets;

};
