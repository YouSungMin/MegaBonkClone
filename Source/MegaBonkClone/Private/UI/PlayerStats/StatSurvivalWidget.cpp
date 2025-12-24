// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerStats/StatSurvivalWidget.h"

#include "Components/TextBlock.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Characters/Components/StatusComponent.h"

void UStatSurvivalWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//if (MaxHP_TitleText)
	//{
	//	MaxHP_TitleText->SetText(FText::FromString(TEXT("최대 HP"))); // 제목은 고정
	//}
	APawn* pawn = GetOwningPlayerPawn();	//getplayerpawn
	if (!pawn) return;
	APlayerCharacter* player = Cast<APlayerCharacter>(pawn);	//casttoPlayerCharacter
	if (!player) return;
	UStatusComponent* status = player->GetStatusComponent();	//getstatusComponent
	if (!status) return;

	//델리게이트 바인딩
	status->OnStatusUpdated.AddDynamic(this, &UStatSurvivalWidget::HandleSurvivalStatusUpdated);
	UE_LOG(LogTemp, Warning, TEXT("델리게이트 바인딩"));


	RefreshSurvival();
}

void UStatSurvivalWidget::NativeDestruct()
{
	APawn* pawn = GetOwningPlayerPawn();
	if (pawn)
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(pawn))
		{
			if (UStatusComponent* status = player->GetStatusComponent())
			{
				//바인딩 해제
				status->OnStatusUpdated.RemoveDynamic(this, &UStatSurvivalWidget::HandleSurvivalStatusUpdated);
				UE_LOG(LogTemp, Warning, TEXT("바인딩 해제"));
			}
		}
	}
	
	Super::NativeDestruct();
}

void UStatSurvivalWidget::HandleSurvivalStatusUpdated()
{
	RefreshSurvival();
}

void UStatSurvivalWidget::RefreshSurvival()
{
	APawn* pawn = GetOwningPlayerPawn();	//getplayerpawn
	if (!pawn) return;
	APlayerCharacter* player = Cast<APlayerCharacter>(pawn);	//casttoPlayerCharacter
	if (!player) return;
	UStatusComponent* status = player->GetStatusComponent();	//getstatusComponent
	if (!status) return;

	const float MaxHP = status->GetResultMaxHP();	//getresult--
	MaxHPValue->SetText(FText::AsNumber(FMath::CeilToFloat(MaxHP)));

	const float HPRegen = status->GetResultHPRegen() * 60.0f;	//getresult--
	HPRegenValue->SetText(FText::AsNumber(FMath::CeilToFloat(HPRegen)));

	const float OverHeal = status->GetResultOverHeal();	//getresult--
	OverHealValue->SetText(FText::AsNumber(FMath::CeilToFloat(OverHeal)));
	
	const float Shield = status->GetResultShield();	//getresult--
	ShieldValue->SetText(FText::AsNumber(FMath::CeilToFloat(Shield)));

	const float Armor = status->GetResultArmor() * 100.0f;	//getresult--
	ArmorValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}%")), 
			FText::AsNumber(FMath::CeilToFloat(Armor))));

	const float Evasion = status->GetResultEvasionChance() * 100.0f;	//getresult--
	EvasionValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}%")), 
			FText::AsNumber(FMath::CeilToFloat(Evasion))));

	const float LifeDrain = status->GetResultLifeDrain();	//getresult--
	LifeDrainValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}%")), 
			FText::AsNumber(FMath::CeilToFloat(LifeDrain))));

	const float Thorn = status->GetResultThorn();	//getresult--
	ThornValue->SetText(FText::AsNumber(FMath::CeilToFloat(Thorn)));

}
