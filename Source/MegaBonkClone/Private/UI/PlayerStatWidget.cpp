// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerStatWidget.h"
#include "Components/TextBlock.h"

void UPlayerStatWidget::SetRowStat(const FText& InLabel, const FText& InValue)
{
	StatLabel->SetText(InLabel);
	StatValue->SetText(InValue);
}
