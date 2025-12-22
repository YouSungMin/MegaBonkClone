// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/InventoryWidget.h"
#include "UI/PlayerStats/PlayerStatsWidget.h"
#include "MainHudWidget.generated.h"

UENUM(BlueprintType)
enum class EOpenState : uint8
{
	Open		UMETA(DisplayName = "Open"),
	Close		UMETA(DisplayName = "Close")
};

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UMainHudWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void OpenPanels();

	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void ClosePanels();

	//인벤토리 열림 상태 GETTER
	inline EOpenState GetOpenState() const { return OpenState; }
	inline UInventoryWidget* GetInventoryWidget() const { return InventoryPanel; }
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UResourceBarWidget> ShieldBar = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UResourceBarWidget> HealthBar = nullptr; //플레이어 HP바

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UInventoryWidget> InventoryPanel = nullptr; //인벤토리 위젯

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStat", meta = (BindWidget))
	TObjectPtr<UPlayerStatsWidget> PlayerStatsPanel = nullptr; //플레이어스탯 위젯


private:
	EOpenState OpenState = EOpenState::Close;	//인벤토리 열림 상태 //기본은 닫힘
};
