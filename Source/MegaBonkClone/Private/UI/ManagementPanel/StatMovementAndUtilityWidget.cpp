// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ManagementPanel/StatMovementAndUtilityWidget.h"

#include "Components/TextBlock.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Characters/Components/StatusComponent.h"


void UStatMovementAndUtilityWidget::NativeConstruct()
{
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
	MoveSpeedValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")), 
			FText::AsNumber(FMath::FloorToInt(MoveSpeed))));

	const float ExtraJump = status->GetResultExtraJump();	//getresult--
	ExtraJumpValue->SetText(FText::AsNumber(FMath::FloorToInt(ExtraJump)));

	const float JumpPower = status->GetResultJumpPower();	//getresult--
	JumpPowerValue->SetText(FText::AsNumber(FMath::FloorToInt(JumpPower)));

}

void UStatMovementAndUtilityWidget::RefreshUtility()
{

	APawn* pawn = GetOwningPlayerPawn();	//getplayerpawn
	if (!pawn) return;
	APlayerCharacter* player = Cast<APlayerCharacter>(pawn);	//casttoPlayerCharacter
	if (!player) return;
	UStatusComponent* status = player->GetStatusComponent();	//getstatusComponent
	if (!status) return;

	const float Luck = status->GetResultLuck();	//getresult--
	LuckValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}%")), 
			FText::AsNumber(FMath::FloorToInt(Luck))));

	const float Difficulty = status->GetResultDifficulty();	//getresult--
	DifficultyValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}%")), 
			FText::AsNumber(FMath::FloorToInt(Difficulty))));

	const float PickupRange = status->GetResultPickUpRange();	//getresult--
	PickupRangeValue->SetText(FText::AsNumber(FMath::FloorToInt(PickupRange)));

	const float ExpGain = status->GetResultExpGain();	//getresult--
	ExpGainValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")), 
			FText::AsNumber(FMath::FloorToInt(ExpGain))));

	const float GoldGain = status->GetResultGoldGain();	//getresult--
	GoldGainValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")), 
			FText::AsNumber(FMath::FloorToInt(GoldGain))));

	const float SilverGain = status->GetResultSilverGain();	//getresult--
	SilverGainValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")), 
			FText::AsNumber(FMath::FloorToInt(SilverGain))));

	const float PowerUpRate = status->GetResultPowerUPRate();	//getresult--
	PowerUpRateValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(FMath::FloorToInt(PowerUpRate))));

	const float DropRate = status->GetResultPowerUPDropRate();	//getresult--
	DropRateValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(FMath::FloorToInt(DropRate))));
}
