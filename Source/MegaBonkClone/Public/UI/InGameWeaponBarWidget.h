// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWeaponBarWidget.generated.h"

class UWeaponSystemComponent;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UInGameWeaponBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	//�ܺ� �κ��丮 ������Ʈ ����
	void InitializeInventoryWidget(UWeaponSystemComponent* WeaponSystemComponent);

	//���� �κ��丮�� UIä���
	UFUNCTION()
	void RefreshInventoryWidget();


	UFUNCTION()
	void ClearInventoryWidget();

private:
	//������ �߰��� �̺�Ʈ ��ü ��������
	UFUNCTION()
	void HandleWeaponChanged();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI|Inventory|Weapon")
	TObjectPtr<class UUniformGridPanel> WeaponSlotGridPanel = nullptr;

private:
	UPROPERTY()
	TWeakObjectPtr<class UWeaponSystemComponent> TargetWeaponInventory = nullptr;


	//NativeConstruct���� �׸����г� 8�� ���� ĳ��
	UPROPERTY()
	TArray<TObjectPtr<class UWeaponSlotWidget>> WeaponSlotWidgets;

};
