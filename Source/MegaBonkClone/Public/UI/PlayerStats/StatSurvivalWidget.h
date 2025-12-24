// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatSurvivalWidget.generated.h"

class UTextBlock; 

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UStatSurvivalWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;	
	virtual void NativeDestruct() override;

	//델리게이트 바인딩될함수
	UFUNCTION()
	void HandleSurvivalStatusUpdated();

	//이벤트만들기
	UFUNCTION(BlueprintCallable)
	void RefreshSurvival();

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label1 = nullptr;	//제목
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxHPValue = nullptr;		//값 

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label2 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HPRegenValue= nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label3 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> OverHealValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label4 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ShieldValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label5 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ArmorValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label6 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EvasionValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label7 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LifeDrainValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label8 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ThornValue = nullptr;

};
