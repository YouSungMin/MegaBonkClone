// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/ChestOpen.h" 
#include "Data/ItemDataStructs.h"
#include "Interactables/MicrowaveActor.h" 
#include "UI/MainHudWidget.h"
#include "MainHUD.generated.h"


/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API AMainHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	inline UMainHudWidget* GetMainWidget() const { return MainWidgetInstance; }

	//외부(상자)에서 호출할 함수
	UFUNCTION(BlueprintCallable)
	void ShowChestReward(const FItemData& ItemData);
	UFUNCTION(BlueprintCallable)
	void ShowRechargeSanctuary(class ARechargeSanctuary* Sanctuary);
	UFUNCTION(BlueprintCallable)
	void ShowShadyStore(class AShadyGuyActor* ShadyGuy, const TArray<struct FShopSlotInfo>& Items);
	UFUNCTION(BlueprintCallable)
	void ShowMicrowave(class AMicrowaveActor* Microwave, const TArray<FMicrowaveSlotInfo>& FilteredList);
	UFUNCTION(BlueprintCallable)
	void ShowUpgrade(class URewardSystemComponent* RewardComp, const TArray<struct FRewardOption>& Options);


	void OpenCenterPanel(TSubclassOf<UUserWidget> PanelClass, TObjectPtr<UUserWidget>& WidgetInstance);

	UFUNCTION(BlueprintCallable)
	void CloseCenterUI();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowGameOver();


protected:
	//mainwidgetclass는 Umainhudwidget를 상속받은 블루프린트 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UMainHudWidget> MainWidgetClass = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UMainHudWidget> MainWidgetInstance = nullptr;

	//게임오버 위젯클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass = nullptr;

	UPROPERTY()
	TObjectPtr<UUserWidget> GameOverWidgetInstance = nullptr;


	//에디터에서 WBP_ChestPopup을 할당할 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Panels")
	TSubclassOf<UChestOpen> ChestOpenClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Panels")
	TSubclassOf<UUserWidget> RechargePanelClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Panels")
	TSubclassOf<UUserWidget> StorePanelClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Panels")
	TSubclassOf<UUserWidget> MicroWavePanelClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Panels")
	TSubclassOf<UUserWidget> UpgradePanelClass;

	//생성된 위젯을 저장해둘 변수
	UPROPERTY()
	TObjectPtr<UChestOpen> ChestOpenWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UUserWidget> RechargeWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UUserWidget> StoreWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UUserWidget> UpgradeWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UUserWidget> MicroWaveWidget = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> CachedStudioActor = nullptr;
};