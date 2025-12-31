// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/AudioManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// Sets default values
AAudioManager::AAudioManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BGMComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMComponent"));
	SetRootComponent(BGMComponent);

	// 맵에 배치만 해도 자동 재생
	BGMComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();
	PlayBGM();
}

void AAudioManager::PlayBGM()
{
	if (!BGMComponent) return;

	if (BGM)
	{
		BGMComponent->SetSound(BGM);
		BGMComponent->Play();
	}
}

void AAudioManager::StopBGM(float FadeOutDuration)
{
	if (!BGMComponent) return;

	if (FadeOutDuration > 0.f)
	{
		BGMComponent->FadeOut(FadeOutDuration, 0.f);
	}
	else
	{
		BGMComponent->Stop();
	}
}

void AAudioManager::PlayCoinSFX()
{
	if (SFX_Coin)	UGameplayStatics::PlaySound2D(this, SFX_Coin);
}

void AAudioManager::PlayUpgradeOpenSFX()
{
	if (SFX_UpgradeOpen)	UGameplayStatics::PlaySound2D(this, SFX_UpgradeOpen, 0.2f);
}

void AAudioManager::PlayFootStepSFX()
{
	if (SFX_FootStep)	UGameplayStatics::PlaySound2D(this, SFX_FootStep);
}

void AAudioManager::PlayJumpStepSFX()
{
	if (SFX_JumpStep)	UGameplayStatics::PlaySound2D(this, SFX_JumpStep);
}

void AAudioManager::PlayGameOverSFX()
{
	if (SFX_GameOver)	UGameplayStatics::PlaySound2D(this, SFX_GameOver);
}

void AAudioManager::SetBGMVolume(float Volume, float FadeTime)
{
	if (!BGMComponent) return;

	// 범위 제한 
	Volume = FMath::Clamp(Volume, 0.f, 2.f);

	if (FadeTime > 0.f)
	{
		// 재생은 유지하고, FadeTime 동안 볼륨만 부드럽게 조절
		BGMComponent->AdjustVolume(FadeTime, Volume);
	}
	else
	{
		// 즉시 볼륨 변경
		BGMComponent->SetVolumeMultiplier(Volume);
	}
}

void AAudioManager::SetBGMDown(bool bEnable, float FadeTime)
{
	// true면 업그레이드 볼륨으로, false면 평상시 볼륨으로
	SetBGMVolume(bEnable ? DownBGMVolume : NormalBGMVolume, FadeTime);
}

