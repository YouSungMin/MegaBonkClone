// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TopDetailWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Characters/Components/StatusComponent.h"
#include "Kismet/GameplayStatics.h"


void UTopDetailWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
		if (Player)
		{
			UStatusComponent* StatusComp = Player->GetStatusComponent();
			if (StatusComp)
			{
				//델리게이트 바인드
				StatusComp->OnGoldChanged.AddDynamic(this, &UTopDetailWidget::RefreshGoldCoinText);
				StatusComp->OnSilverChanged.AddDynamic(this, &UTopDetailWidget::RefreshSilverCoinText);
				StatusComp->OnKillCountChanged.AddDynamic(this, &UTopDetailWidget::RefreshKillCountText);
				StatusComp->OnLevelChanged.AddDynamic(this, &UTopDetailWidget::RefreshLevelText);
				StatusComp->OnStatExpChanged.AddDynamic(this, &UTopDetailWidget::RefreshExpProgressbar);
				//초기값 초기화
				InitializeTopDetail(StatusComp);
			}
		}
	}
}

void UTopDetailWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//월드 
	UWorld* World = GetWorld();
	if (World)
	{
		float TimeSeconds = World->GetTimeSeconds();

		// 분/초 계산
		int32 Minutes = FMath::FloorToInt(TimeSeconds / 60.0f);
		int32 Seconds = FMath::FloorToInt(TimeSeconds) % 60;

		// 위젯 갱신
		RefreshGameTimeText((float)Minutes, (float)Seconds);
		// 필요하다면 TimerMinText도 여기서 같이 갱신
		//RefreshTimerText((float)Minutes, (float)Seconds);
	}
}

void UTopDetailWidget::InitializeTopDetail(UStatusComponent* StatusComp)
{
	

	RefreshGoldCoinText(StatusComp->GetCurrentGold());
	RefreshSilverCoinText(StatusComp->GetCurrentSilver());
	RefreshKillCountText(StatusComp->GetKillCount());
	RefreshLevelText(static_cast<float>(StatusComp->GetCurrentLevel()));
	RefreshExpProgressbar(StatusComp->GetCurrentExp(), StatusComp->GetMaxExp());
}

void UTopDetailWidget::RefreshTimerText(float InMin, float InSec)
{

	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumIntegralDigits = 2;
	NumberFormat.MaximumIntegralDigits = 2;


	FText minText = FText::AsNumber(InMin , &NumberFormat);
	FText secText = FText::AsNumber(InSec, &NumberFormat);
	
	if (minText.IsNumeric()) {
		TimerMinText->SetText(minText);
	}
	

	if (secText.IsNumeric()) {
		TimerSecText->SetText(secText);
	}
	
}

void UTopDetailWidget::RefreshGoldCoinText(float InGold)
{
	FText goldCoinText = FText::AsNumber(InGold);

	GoldCoinText->SetText(goldCoinText);
}

void UTopDetailWidget::RefreshSilverCoinText(float InSilver)
{
	FText silverCoinText = FText::AsNumber(InSilver);

	SilverCoinText->SetText(silverCoinText);
}

void UTopDetailWidget::RefreshKillCountText(float InKillCount)
{
	FText killCountText = FText::AsNumber(InKillCount);

	KillCountText->SetText(killCountText);
}

void UTopDetailWidget::RefreshGameTimeText(float InMin, float InSec)
{
	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumIntegralDigits = 2;
	NumberFormat.MaximumIntegralDigits = 2;


	FText minText = FText::AsNumber(InMin, &NumberFormat);
	FText secText = FText::AsNumber(InSec, &NumberFormat);

	if (minText.IsNumeric()) {
		GameTimeMinText->SetText(minText);
	}


	if (secText.IsNumeric()) {
		GameTimeSecText->SetText(secText);
	}
}

void UTopDetailWidget::RefreshLevelText(float InLevel)
{
	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumIntegralDigits = 2;
	NumberFormat.MaximumIntegralDigits = 2;

	FText levelText = FText::AsNumber(InLevel, &NumberFormat);

	if (levelText.IsNumeric()) {
		LevelText->SetText(levelText);
	}
	
}

void UTopDetailWidget::RefreshExpProgressbar(float InCurrentExp, float InMaxExp)
{
	float expProgressRatio = InCurrentExp / InMaxExp;

	ExpProgressbar->SetPercent(expProgressRatio);
}

