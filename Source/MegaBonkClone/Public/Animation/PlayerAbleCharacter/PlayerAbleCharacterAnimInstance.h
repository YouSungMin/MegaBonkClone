// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAbleCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UPlayerAbleCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimInstance|Speed")
	float Speed=0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimInstance|Speed")
	bool IsJumping = false;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimInstance")
	TWeakObjectPtr<class UPawnMovementComponent> MovementComponent = nullptr;

};
