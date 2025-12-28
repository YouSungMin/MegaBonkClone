// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MicroSlotWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UMicroSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RechargeSlot", meta = (BindWidget))
	TObjectPtr<UImage> Image = nullptr;	//값
};
