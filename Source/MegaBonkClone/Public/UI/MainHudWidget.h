// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UMainHudWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TWeakObjectPtr<class UResourceBarWidget> ShieldBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TWeakObjectPtr<class UResourceBarWidget> HealthBar; //플레이어 HP바

};
