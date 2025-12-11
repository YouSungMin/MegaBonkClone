// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PlayerAbleCharacter/PlayerAbleCharacterAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
void UPlayerAbleCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//무브먼트 컴포넌트 가져오기
	if (APawn* ownerPawn = TryGetPawnOwner()) {
		MovementComponent = ownerPawn->GetMovementComponent();
	}
	
}

void UPlayerAbleCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (MovementComponent.IsValid()) {
		//Velocity의 길이 속도 캐릭터의 전체속도
		Speed = MovementComponent.Get()->Velocity.Size();
		//UE_LOG(LogTemp, Warning, TEXT("%.1f"),MovementComponent.Get()->Velocity.Z);
		
		//수직속도 : 낙하 점프중 상태체크용
		ZSpeed = MovementComponent.Get()->Velocity.Z;
	}
}
