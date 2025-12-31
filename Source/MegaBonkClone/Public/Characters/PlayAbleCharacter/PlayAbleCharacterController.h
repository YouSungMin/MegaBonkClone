// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/MainHudWidget.h"
#include "PlayAbleCharacterController.generated.h"

struct FInputActionValue;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API APlayAbleCharacterController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;
	//포세스 될 때 실행할 함수
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;	

	//카메라 이동 입력 받으면 실행할 함수
	UFUNCTION()
	void OnCameraLookInput(const FInputActionValue& InValue);

	virtual void SetupInputComponent() override;

public:
	void OpenPanels();
	UFUNCTION()
	void ClosePanels();

	//메인 HUD 위젯 세터
	void InitializeMainHudWidget(UMainHudWidget* InWidget);


private:
	//패널들 온오프 입력 받으면 실행할 함수(인벤토리, 플레이어스탯)
	void OnPanelOnOff();

protected:

	//입력 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerController|Input")
	TObjectPtr<class UInputAction> IA_CameraLook = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerController|Input")
	TObjectPtr<class UInputAction> IA_PanelOnOff= nullptr;

	//입력 매핑 컨텍스트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerController|Input")
	TObjectPtr<class UInputMappingContext> DefaultInputMappingContext = nullptr;

private:
	TWeakObjectPtr<UMainHudWidget> MainHudWidget = nullptr;
	TWeakObjectPtr<UInventoryWidget> InventoryWidget = nullptr;
	TWeakObjectPtr<class UInventoryComponent> InventoryComponent = nullptr;
};
