// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/MicroSlotWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Interactables/MicrowaveActor.h" 


void UMicroSlotWidget::NativeConstruct()
{
	if (SlotButton)
	{
		SlotButton->OnClicked.RemoveDynamic(this, &UMicroSlotWidget::HandleClicked);
		SlotButton->OnClicked.AddDynamic(this, &UMicroSlotWidget::HandleClicked);
	}
}

void UMicroSlotWidget::SetSlotInfo(const FMicrowaveSlotInfo& Info)
{
	CachedItemID = Info.ItemID;

	if (Image)
	{
		Image->SetBrushFromTexture(Info.Icon, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[MicroSlot] Icon is NULL for %s"), *Info.ItemID.ToString());
	}
}

void UMicroSlotWidget::HandleClicked()
{
	if (CachedItemID.IsNone()) return;
	OnSlotClicked.Broadcast(CachedItemID);
}
