// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatMovementAndUtilityWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UStatMovementAndUtilityWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	//�̺�Ʈ�����
	UFUNCTION(BlueprintCallable)
	void RefreshMovement();		//�̵�
	void RefreshUtility();	//��ƿ��Ƽ


	UFUNCTION()
	void HandleUtilityStatusUpdated();


protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label1 = nullptr;	//����
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveSpeedValue = nullptr;		//�� 

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label2 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ExtraJumpValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label3 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> JumpPowerValue = nullptr;


	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label4 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LuckValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label5 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DifficultyValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label6 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PickupRangeValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label7 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ExpGainValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label8 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldGainValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label9 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SilverGainValue = nullptr;


	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label11 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PowerUpRateValue = nullptr;


	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label12 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DropRateValue = nullptr;

	//UPROPERTY(meta = (BindWidgetOptional))
	//TObjectPtr<UTextBlock> Label10 = nullptr;
	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<UTextBlock> EliteSpawnRateValue = nullptr;

};

