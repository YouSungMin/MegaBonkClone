// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/CursedSanctuary.h"
#include "Framework/MegaBonkGameState.h"
#include <Kismet/GameplayStatics.h>


void ACursedSanctuary::ApplyEffect_Implementation(AActor* Player)
{
    // 1. 월드에서 현재 GameState를 가져옵니다.
    AGameStateBase* BaseGameState = UGameplayStatics::GetGameState(this);

    // 2. 우리가 만든 MyGameState로 형변환(Cast) 합니다.
    if (AMegaBonkGameState* MyGameState = Cast<AMegaBonkGameState>(BaseGameState))
    {
        // 3. 카운트 증가 함수 호출
        MyGameState->IncreaseBossSummonCount();
        SanNotifyToHUD(FString::Printf(TEXT("저주의 제단 활성화됨!")));
        Destroy();
        // (선택) 제단 비활성화나 이펙트 재생 로직
        UE_LOG(LogTemp, Log, TEXT("저주의 제단 활성화됨!"));
    }
}
