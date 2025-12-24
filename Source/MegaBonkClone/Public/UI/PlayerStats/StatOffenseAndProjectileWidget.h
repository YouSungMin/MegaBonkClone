// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatOffenseAndProjectileWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UStatOffenseAndProjectileWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	//�̺�Ʈ�����
	UFUNCTION(BlueprintCallable)
	void RefreshOffense();	//����
	void RefreshProjectile();	//�߻�ü

	UFUNCTION()
	void HandleOffenseStatusUpdated();



protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label1 = nullptr;	//����
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageValue = nullptr;		//�� 

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label2 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CritChanceValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label3 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CritDmgValue = nullptr;


	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label4 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AtkSpeedValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label5 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CountValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label6 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ReflectValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label7 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SizeValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label8 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ProjSpeedValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label9 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DurationValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label10 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EliteDmgValue = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Label11 = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KnockbackValue = nullptr;

};
