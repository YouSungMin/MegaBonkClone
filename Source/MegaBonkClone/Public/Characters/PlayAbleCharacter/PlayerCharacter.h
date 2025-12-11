// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MEGABONKCLONE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

public:

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Components")
	TObjectPtr<class USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Components")
	TObjectPtr<class UCameraComponent> Camera = nullptr;


private:


};
