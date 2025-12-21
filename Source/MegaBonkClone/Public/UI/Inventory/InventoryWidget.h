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

	UPROPERTY(BlueprintAssignable, Category = "UI|Inventory")
	FOnInventoryCloseRequested OnInventoryCloseRequested;

private:
	UFUNCTION()
	void OnCloseInventoryClicked();

protected:
	//UPROPERTY(meta = (BindWIdget))
	//TObjectPtr<class UButton> CloseButton = nullptr;

};
