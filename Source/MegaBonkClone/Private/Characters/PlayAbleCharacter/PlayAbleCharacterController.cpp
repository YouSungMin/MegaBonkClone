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

	UE_LOG(LogTemp, Warning, TEXT("%s"),*input.ToString());
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
	}

}