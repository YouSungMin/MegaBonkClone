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

	APawn* pawn = GetOwningPlayerPawn();	//getplayerpawn
	if (!pawn) return;
	APlayerCharacter* player = Cast<APlayerCharacter>(pawn);	//casttoPlayerCharacter
	if (!player) return;
	UStatusComponent* status = player->GetStatusComponent();	//getstatusComponent
	if (!status) return;

	//델리게이트 바인딩
	status->OnStatusUpdated.AddDynamic(this, &UStatOffenseAndProjectileWidget::HandleOffenseStatusUpdated);
	UE_LOG(LogTemp, Warning, TEXT("델리게이트 바인딩"));
}

void UStatOffenseAndProjectileWidget::RefreshOffense()
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

	const float Damage = status->GetResultDamage();	//getresult--
	DamageValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(Damage, &NumberFormat)));

	const float CritChance = status->GetResultCriticalChance()*100.0f;	//getresult--
	CritChanceValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}%")),
			FText::AsNumber(FMath::FloorToInt(CritChance))));

	const float CritDmg = status->GetResultCritDmgRate();	//getresult--
	CritDmgValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(CritDmg, &NumberFormat)));

	const float AtkSpeed = status->GetResultAttackSpeed()*100.0f;	//getresult--
	AtkSpeedValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}%")),
			FText::AsNumber(FMath::FloorToInt(AtkSpeed))));
}

void UStatOffenseAndProjectileWidget::RefreshProjectile()
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

	const float Count = status->GetResultProjectileCount();	//getresult--
	CountValue->SetText(FText::AsNumber(FMath::FloorToInt(Count)));

	const float Reflect = status->GetResultProjectileReflectCount();	//getresult--
	ReflectValue->SetText(FText::AsNumber(FMath::FloorToInt(Reflect)));

	const float Size = status->GetResultAttackSize();	//getresult--
	SizeValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(Size, &NumberFormat)));

	const float ProjSpeed = status->GetResultProjectileSpeed();	//getresult--
	ProjSpeedValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(ProjSpeed, &NumberFormat)));

	const float Duration = status->GetResultAttackDuration();	//getresult--
	DurationValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(Duration, &NumberFormat)));

	const float EliteDmg = status->GetResultEliteDamage();	//getresult--
	EliteDmgValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(EliteDmg, &NumberFormat)));

	const float Knockback = status->GetResultKnockBack();	//getresult--
	KnockbackValue->SetText(
		FText::Format(FText::FromString(TEXT("{0}x")),
			FText::AsNumber(Knockback, &NumberFormat)));

}

void UStatOffenseAndProjectileWidget::HandleOffenseStatusUpdated()
{
	RefreshOffense();
	RefreshProjectile();
}

