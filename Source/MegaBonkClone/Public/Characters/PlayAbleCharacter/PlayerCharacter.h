// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;

UCLASS()
class MEGABONKCLONE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	//캐릭터 초기 설정 함수
	void InitializeCharacterComponents();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//이동 입력 받으면 실행할 함수
	UFUNCTION()
	void OnMoveInput(const FInputActionValue& InValue);
	//점프 입력 받으면 실행할 함수
	UFUNCTION()
	void OnJumpInput(const FInputActionValue& InValue);
	

private:

public:

protected:

	//입력 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
	TObjectPtr<class UInputAction> IA_Move = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
	TObjectPtr<class UInputAction> IA_Jump = nullptr;

	


	//컴포넌트들 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Components")
	TObjectPtr<class USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Components")
	TObjectPtr<class UCameraComponent> Camera = nullptr;


private:


};
