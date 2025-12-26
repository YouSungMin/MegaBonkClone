// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageValueWidget.h"
#include "Components/TextBlock.h"

void UDamageValueWidget::PlayDamagePopup(float Damage)
{
	if (DamageText)
	{
		DamageText->SetText(FText::AsNumber(static_cast<int32>(Damage)));
	}

	if (DamagePopupAnimation)
	{
		StopAnimation(DamagePopupAnimation);
		PlayAnimation(DamagePopupAnimation); //1È¸ Àç»ý
	}
}
