// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerStats/StatMovementAndUtilityWidget.h"

#include "Components/TextBlock.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Characters/Components/StatusComponent.h"


void UStatMovementAndUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APawn* pawn = GetOwningPlayerPawn();	//getplayerpawn
	if (!pawn) return;
	APlayerCharacter* player = Cast<APlayerCharacter>(pawn);	//casttoPlayerCharacter
	if (!player) return;
	UStatusComponent* status = player->GetStatusComponent();	//getstatusComponent
	if (!status) return;

	//델리게이트 바인딩
	status->OnStatusUpdated.AddDynamic(this, &UStatMovementAndUtilityWidget::HandleUtilityStatusUpdated);
	UE_LOG(LogTemp, Warning, TEXT("델리게이트 바인딩"));

	RefreshMovement();
	RefreshUtility();
}

void UStatMovementAndUtilityWidget::RefreshMovement()
{

	APawn* pawn = GetOwningPlayerPawn();	//getplayerpawn
	if (!pawn) return;
	APlayerCharacter* player = Cast<APlayerCharacter>(pawn);	//casttoPlayerCharacter
	if (!player) return;
	UStatusComponent* status = player->GetStatusComponent();	//getstatusComponent
	if (!status) return;

	const float MoveSpeed = status->GetResultMoveSpeed();	//getresult--
	MoveSpeedValue->SetText(FText::AsNumber(FMath::FloorToInt(MoveSpeed)));

	const float ExtraJump = status->GetResultExtraJump();	//getresult--
	ExtraJumpValue->SetText(FText::AsNumber(FMath::FloorToInt(ExtraJump)));

	const float JumpPower = status->GetResultJumpPower();	//getresult--
	JumpPowerValue->SetText(FText::AsNumber(FMath::FloorToInt(JumpPower)));

}

void UStatMovementAndUtilityWidget::RefreshUtility()
{
	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumFractionalDigits = 1;
	NumberFormat.MaximumFractionalDigits = 1;

	APawn* pawn = GetOwningPlayerPawn();	//getplayerpawn
	if (!pawn) return;
	APlayerCharacter* player = Cast<APlayerCharacter>(pawn);	//casttoPlayerCharacter
	if (!player) return;
	UStatusComponent* status = player->GetStatusComponent();	//getstatusComponent
	if (!status) return;

	const float Luck = status->GetResultLuck();	//getresult--
	LuckValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}%")), 
			FText::AsNumber(FMath::CeilToFloat(Luck))));

	const float Difficulty = status->GetResultDifficulty() * 100.0f;	//getresult--
	DifficultyValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}%")), 
			FText::AsNumber(FMath::CeilToFloat(Difficulty))));

	const float PickupRange = status->GetResultPickUpRange();	//getresult--
	PickupRangeValue->SetText(FText::AsNumber(FMath::FloorToInt(PickupRange)));

	const float ExpGain = status->GetResultExpGain();	//getresult--
	ExpGainValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(ExpGain, &NumberFormat)));

	const float GoldGain = status->GetResultGoldGain();	//getresult--
	GoldGainValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(GoldGain, &NumberFormat)));

	const float SilverGain = status->GetResultSilverGain();	//getresult--
	SilverGainValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(SilverGain, &NumberFormat)));

	const float PowerUpRate = status->GetResultPowerUPRate();	//getresult--
	PowerUpRateValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(PowerUpRate, &NumberFormat)));

	const float DropRate = status->GetResultPowerUPDropRate();	//getresult--
	DropRateValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(DropRate, &NumberFormat)));
}

void UStatMovementAndUtilityWidget::HandleUtilityStatusUpdated()
{
	RefreshMovement();		
	RefreshUtility();
}
