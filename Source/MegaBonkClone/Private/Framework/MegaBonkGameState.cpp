// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MegaBonkGameState.h"

AMegaBonkGameState::AMegaBonkGameState()
{
	BossSummonCount = 1;
	AliveBossCount = 0;
}

void AMegaBonkGameState::OnBossDied()
{
	AliveBossCount--;

	// 디버깅용 로그
	UE_LOG(LogTemp, Warning, TEXT("보스 사망! 남은 보스: %d"), AliveBossCount);

	// 남은 보스가 0 이하라면?
	if (AliveBossCount <= 0)
	{
		AliveBossCount = 0; // 혹시 모를 음수 방지

		if (OnAllBossesDead.IsBound())
		{
			OnAllBossesDead.Broadcast();
			UE_LOG(LogTemp, Warning, TEXT("모든 보스 처치 완료! 다음 스테이지 개방 신호 전송!"));
		}
	}
}