// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/InventoryWidget.h"
#include "UI/InGameItemBarWidget.h"
#include "UI/InGameWeaponBarWidget.h"
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

	//중앙 패널을 교체하는 함수
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetCenterContent(UUserWidget* NewWidget);

	//중앙 패널을 비우는 함수
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ClearCenterContent();

	//업그레이드 애니메이션 재생함수
	UFUNCTION(BlueprintCallable)
	void SetUpgradeAnimEnabled(bool bEnabled);

	//알림 텍스트
	UFUNCTION(BlueprintCallable, Category = "UI")
	void PlayNotificationText(FText Message);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ClearNotificationText();

private:
	//비전시 슬롯 캐싱
	void CacheSecretSlots();
	void RefreshSecretSlots();

	// 인벤 변하면 갱신
	UFUNCTION()
	void HandleInventoryChanged(FName ItemID, const FItemData& ItemData);


	UFUNCTION()
	void HandleHPChanged(float CurrentHP, float MaxHP);

	UFUNCTION()
	void HandleShieldChanged(float CurrentShield, float MaxShield);



public:
	//인벤토리 열림 상태 GETTER
	inline EOpenState GetOpenState() const { return OpenState; }
	inline UInventoryWidget* GetInventoryWidget() const { return InventoryPanel; }
	inline UInGameItemBarWidget* GetItemBarWidget() const { return ItemBar; }
	inline UInGameWeaponBarWidget* GetWeaponBarWidget() const { return WeaponBar;}
	
private:
	UPROPERTY()
	TObjectPtr<UInventoryComponent> CachedInventory = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<class USecretBookSlotWidget>> SecretSlotWidgets;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Notification")
	TObjectPtr<class UTextBlock> NoticeText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UResourceBarWidget> ShieldBar = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UResourceBarWidget> HealthBar = nullptr; //플레이어 HP바

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UInventoryWidget> InventoryPanel = nullptr; //인벤토리 위젯

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStat", meta = (BindWidget))
	TObjectPtr<UPlayerStatsWidget> PlayerStatsPanel = nullptr; //플레이어스탯 위젯

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item", meta = (BindWidget))
	TObjectPtr<UInGameItemBarWidget> ItemBar = nullptr; //플레이어스탯 위젯

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (BindWidget))
	TObjectPtr<UInGameWeaponBarWidget> WeaponBar = nullptr; //플레이어스탯 위젯

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI|Inventory|Items")
	TObjectPtr<class UUniformGridPanel> SecretSlotGridPanel = nullptr;
	//에디터의 Named Slot과 바인딩
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Center",meta = (BindWidget))
	TObjectPtr<UNamedSlot> CenterSlot = nullptr;

	//업그레이드 오버레이 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> LightOverlay;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> LightAnim;

	FTimerHandle NoticeTimerHandle;


private:
	EOpenState OpenState = EOpenState::Close;	//인벤토리 열림 상태 //기본은 닫힘
};
