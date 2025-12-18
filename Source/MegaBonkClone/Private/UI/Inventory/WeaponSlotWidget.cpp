// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/WeaponSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"

void UWeaponSlotWidget::InitializeSlot(int32 InIndex, FInvenSlot* InSlotData)
{
	Index = InIndex;
	SlotData = InSlotData;

	RefreshWeaponSlot();
}

void UWeaponSlotWidget::RefreshWeaponSlot() const
{
	//if (SlotData && !SlotData->IsEmpty())
	//{
	//	WeaponIconImage->SetBrushFromTexture(SlotData->ItemData->ItemIcon);
	//	
	//	LevelText->SetText(FText::AsNumber(SlotData->GetCount()));

	//}

}

void UWeaponSlotWidget::ClearWeaponSlotWidget() const
{
}
