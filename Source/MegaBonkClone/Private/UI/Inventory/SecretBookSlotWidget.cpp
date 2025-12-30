// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SecretBookSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Components/InventoryComponent.h"

void USecretBookSlotWidget::InitializeSecretBookSlot(FName InItemID, int32 InLevel, UInventoryComponent* InInventory)
{
    ItemID = InItemID;
    Level = InLevel;
    TargetSecretBookInventory = InInventory;

    RefreshSecretBookSlot();
}

void USecretBookSlotWidget::RefreshSecretBookSlot()
{
    if (!TargetSecretBookInventory.IsValid() || ItemID.IsNone() || Level <= 0)
    {
        ClearSecretBookSlot();
        return;
    }

    FItemData* ItemInfo = TargetSecretBookInventory->GetItemInfo(ItemID);
    if (!ItemInfo)
    {
        ClearSecretBookSlot();
        return;
    }

    UTexture2D* Tex = ItemInfo->Icon.LoadSynchronous();
    SecretIconImage->SetBrushFromTexture(Tex);
    SecretIconImage->SetBrushTintColor(FLinearColor::White);

    LevelText->SetText(FText::FromString(FString::Printf(TEXT("LV%d"), Level)));
    //LevelText->SetText(FText::AsNumber(Level));
    LevelText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

}

void USecretBookSlotWidget::ClearSecretBookSlot()
{
    if (SecretIconImage)
    {
        SecretIconImage->SetBrushFromTexture(nullptr);
        SecretIconImage->SetBrushTintColor(FLinearColor::Transparent);
    }

    if (LevelText)
    {
        LevelText->SetVisibility(ESlateVisibility::Hidden);
    }
}
