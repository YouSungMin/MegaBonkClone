// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioManager.generated.h"

class UAudioComponent;
class USoundBase;
UCLASS()
class MEGABONKCLONE_API AAudioManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAudioManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// BGM 제어
	UFUNCTION(BlueprintCallable, Category = "Audio|BGM")
	void PlayBGM();

	UFUNCTION(BlueprintCallable, Category = "Audio|BGM")
	void StopBGM(float FadeOutDuration = 0.5f);


	// SFX 재생
	UFUNCTION(BlueprintCallable, Category = "Audio|SFX")
	void PlayCoinSFX();

	UFUNCTION(BlueprintCallable, Category = "Audio|SFX")
	void PlayUpgradeOpenSFX();

	UFUNCTION(BlueprintCallable, Category = "Audio|SFX")
	void PlayFootStepSFX();

	UFUNCTION(BlueprintCallable, Category = "Audio|SFX")
	void PlayJumpStepSFX();

	UFUNCTION(BlueprintCallable, Category = "Audio|SFX")
	void PlayGameOverSFX();


	// BGM 볼륨을 변경 함수
	UFUNCTION(BlueprintCallable, Category = "Audio|BGM")
	void SetBGMVolume(float Volume, float FadeTime = 0.0f);
	// 업그레이드 UI 에서 BGM (볼륨 낮추기) ON/OFF
	UFUNCTION(BlueprintCallable, Category = "Audio|BGM")
	void SetBGMDown(bool bEnable, float FadeTime = 0.15f);

public:
	// 배경 음악
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio|BGM")
	TObjectPtr<USoundBase> BGM;
	// 코인 획득 사운드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio|SFX")
	TObjectPtr<USoundBase> SFX_Coin;
	// 업그레이드 창 열기 사운드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio|SFX")
	TObjectPtr<USoundBase> SFX_UpgradeOpen;
	// 걷기 발자국
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio|SFX")
	TObjectPtr<USoundBase> SFX_FootStep;
	// 점프 발자국
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio|SFX")
	TObjectPtr<USoundBase> SFX_JumpStep;
	// 게임 오버 사운드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio|SFX")
	TObjectPtr<USoundBase> SFX_GameOver;



public:
	// 평상시 BGM 볼륨 (기본 1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|BGM")
	float NormalBGMVolume = 0.6f;

	// 창이 떠있는 동안의 BGM 볼륨 낮춤 (0.25)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|BGM")
	float DownBGMVolume = 0.1f;

private:
	// BGM 전용 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	TObjectPtr<UAudioComponent> BGMComponent;
};
