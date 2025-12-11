// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PlayerAbleCharacter/PlayerAbleCharacterAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
void UPlayerAbleCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (APawn* ownerPawn = TryGetPawnOwner()) {
		MovementComponent = ownerPawn->GetMovementComponent();
	}
	
}

void UPlayerAbleCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (MovementComponent.IsValid()) {
		Speed = MovementComponent.Get()->Velocity.Size();

		//MovementComponent.Get()->air
	}
}
