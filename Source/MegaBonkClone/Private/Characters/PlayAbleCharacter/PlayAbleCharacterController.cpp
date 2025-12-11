// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayAbleCharacter/PlayAbleCharacterController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void APlayAbleCharacterController::BeginPlay()
{
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
	AddPitchInput(-input.Y);
	AddYawInput(input.X);
}

void APlayAbleCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(InputComponent);

	if (enhanced) {
		enhanced->BindAction(IA_CameraLook, ETriggerEvent::Triggered, this, "OnCameraLookInput");
	}

}