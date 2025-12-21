// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayAbleCharacter/PlayAbleCharacterController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void APlayAbleCharacterController::BeginPlay()
{
	//매핑컨텍스트 추가를 위한 서브시스템 가져오기
	UEnhancedInputLocalPlayerSubsystem* subsystem = 
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (subsystem) {
		subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		//UE_LOG(LogTemp, Warning, TEXT("AddMappingContext 완료"));
	}
}

void APlayAbleCharacterController::OnCameraLookInput(const FInputActionValue& InValue)
{
	FVector2D input = InValue.Get<FVector2D>();

	//UE_LOG(LogTemp, Warning, TEXT("%s"),*input.ToString());
	//마우스 축에 따라 카메라 이동을 위한 코드
	AddPitchInput(-input.Y);
	AddYawInput(input.X);
}

void APlayAbleCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(InputComponent);


	//입력 액션 바인드 함수 
	if (enhanced) {
		enhanced->BindAction(IA_CameraLook, ETriggerEvent::Triggered, this, &APlayAbleCharacterController::OnCameraLookInput);
		enhanced->BindAction(IA_PanelOnOff, ETriggerEvent::Started, this, &APlayAbleCharacterController::OnPanelOnOff);
	}

}


void APlayAbleCharacterController::OpenPanels()
{
	if (MainHudWidget.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("OpenPanels 실행"));
		MainHudWidget->OpenPanels();

		FInputModeGameAndUI inputMode;
		//inputMode.SetWidgetToFocus(MainHudWidget->TakeWidget()); //포커스 설정(선택사항)
		inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); //마우스 잠금 설정
		inputMode.SetHideCursorDuringCapture(false); //마우스 캡처시 커서 숨기지 않음
		SetInputMode(inputMode); //입력 모드를 컨트롤러에 적용

		bShowMouseCursor = true; //마우스 커서 보이기

		SetPause(true); //게임 일시정지
	}
}

void APlayAbleCharacterController::ClosePanels()
{
	if(MainHudWidget.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("ClosePanels 실행"));

		SetPause(false); //게임 일시정지 해제

		FInputModeGameOnly inputMode;
		SetInputMode(inputMode); //입력 모드를 컨트롤러에 적용

		bShowMouseCursor = false; //마우스 커서 보이기

		MainHudWidget->ClosePanels();
	}
}

void APlayAbleCharacterController::OnPanelOnOff()
{
	if(MainHudWidget.IsValid())
	{
		if (MainHudWidget->GetOpenState() == EOpenState::Open)
		{
			ClosePanels();
		}
		else
		{
			OpenPanels();
		}
	}
}