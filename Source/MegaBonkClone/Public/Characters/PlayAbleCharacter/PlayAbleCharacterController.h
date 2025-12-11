// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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

	//카메라 이동 입력 받으면 실행할 함수
	UFUNCTION()
	void OnCameraLookInput(const FInputActionValue& InValue);

	virtual void SetupInputComponent() override;
protected:

	//입력 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerController|Input")
	TObjectPtr<class UInputAction> IA_CameraLook = nullptr;

	//입력 매핑 컨텍스트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerController|Input")
	TObjectPtr<class UInputMappingContext> DefaultInputMappingContext = nullptr;

};
