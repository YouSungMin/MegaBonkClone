// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/MicroShopWidget.h"

void UMicroShopWidget::InitWithMicrowave(AMicrowaveActor* InMicrowave, const TArray<FMicrowaveSlotInfo>& InList)
{

	MicrowaveRef = InMicrowave;
	CachedList = InList;

	UE_LOG(LogTemp, Warning, TEXT("[MicroShop] InitWithMicrowave: %d items"), CachedList.Num());
}
