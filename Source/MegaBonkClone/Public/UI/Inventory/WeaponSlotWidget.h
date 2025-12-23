// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/WeaponDataStructs.h"
#include "WeaponSlotWidget.generated.h"

class UWeaponSystemComponent;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UWeaponSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//�� ������ ������ ������ ����
	void InitializeWeaponSlot(FName InWeaponID, int32 InLevel, UWeaponSystemComponent* InWeapon);

	//������ ������ ����� ������ ǥ���ϴ� ���� ����
	void RefreshWeaponSlot();

protected:
	void ClearWeaponSlot();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|WeaponSlot", meta = (BindWidget))
	TObjectPtr<class UImage> WeaponIconImage = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|WeaponSlot", meta = (BindWidget))
	TObjectPtr<class UTextBlock> LevelText = nullptr;

private:
	FName WeaponID = NAME_None;
	int32 Level = 1;

	UPROPERTY()
	TWeakObjectPtr<UWeaponSystemComponent> TargetWeaponInventory;
};
