// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerStats/StatOffenseAndProjectileWidget.h"

#include "Components/TextBlock.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Characters/Components/StatusComponent.h"


void UStatOffenseAndProjectileWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshOffense();
	RefreshProjectile();
}

void UStatOffenseAndProjectileWidget::RefreshOffense()
{

	APawn* pawn = GetOwningPlayerPawn();	//getplayerpawn
	if (!pawn) return;
	APlayerCharacter* player = Cast<APlayerCharacter>(pawn);	//casttoPlayerCharacter
	if (!player) return;
	UStatusComponent* status = player->GetStatusComponent();	//getstatusComponent
	if (!status) return;

	const float Damage = status->GetResultDamage();	//getresult--
	DamageValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(FMath::FloorToInt(Damage))));

	const float CritChance = status->GetResultCriticalChance();	//getresult--
	CritChanceValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}%")),
			FText::AsNumber(FMath::FloorToInt(CritChance))));

	const float CritDmg = status->GetResultCritDmgRate();	//getresult--
	CritDmgValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(FMath::FloorToInt(CritDmg))));

	const float AtkSpeed = status->GetResultAttackSpeed();	//getresult--
	AtkSpeedValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}%")),
			FText::AsNumber(FMath::FloorToInt(AtkSpeed))));

}

void UStatOffenseAndProjectileWidget::RefreshProjectile()
{

	APawn* pawn = GetOwningPlayerPawn();	//getplayerpawn
	if (!pawn) return;
	APlayerCharacter* player = Cast<APlayerCharacter>(pawn);	//casttoPlayerCharacter
	if (!player) return;
	UStatusComponent* status = player->GetStatusComponent();	//getstatusComponent
	if (!status) return;

	const float Count = status->GetResultProjectileCount();	//getresult--
	CountValue->SetText(FText::AsNumber(FMath::FloorToInt(Count)));

	const float Reflect = status->GetResultProjectileReflectCount();	//getresult--
	ReflectValue->SetText(FText::AsNumber(FMath::FloorToInt(Reflect)));

	const float Size = status->GetResultAttackSize();	//getresult--
	SizeValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(FMath::FloorToInt(Size))));

	const float ProjSpeed = status->GetResultProjectileSpeed();	//getresult--
	ProjSpeedValue ->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(FMath::FloorToInt(ProjSpeed))));

	const float Duration = status->GetResultAttackDuration();	//getresult--
	DurationValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(FMath::FloorToInt(Duration))));

	const float EliteDmg = status->GetResultEliteDamage();	//getresult--
	EliteDmgValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(FMath::FloorToInt(EliteDmg))));

	const float Knockback = status->GetResultKnockBack();	//getresult--
	KnockbackValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")), 
			FText::AsNumber(FMath::FloorToInt(Knockback))));

}

