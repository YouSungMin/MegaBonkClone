// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageValueWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UDamageValueWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	float PlayDamagePopup(float Damage);
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageText = nullptr;;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> DamagePopupAnimation = nullptr;

};
