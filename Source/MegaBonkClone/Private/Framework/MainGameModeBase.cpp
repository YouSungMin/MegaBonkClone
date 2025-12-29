// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MainGameModeBase.h"
#include "GameFramework/Character.h" 

UClass* AMainGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    //목록에 캐릭터가 하나라도 등록되어 있다면?
    if (CharacterOptions.Num() > 0)
    {
        //0번부터 마지막 번호 사이에서 랜덤 숫자 뽑기
        int32 RandomIndex = FMath::RandRange(0, CharacterOptions.Num() - 1);

        //당첨된 캐릭터 클래스 반환
        return CharacterOptions[RandomIndex];
    }

    // 목록이 비어있으면 원래 설정된 DefaultPawnClass 사용
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}