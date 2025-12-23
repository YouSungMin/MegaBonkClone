// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/ItemSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Components/InventoryComponent.h"

void UItemSlotWidget::InitializeItemSlot(FName InItemID, int32 InQuantity, UInventoryComponent* InInventory)
{
    ItemID = InItemID;
    Quantity = InQuantity;
    TargetItemInventory = InInventory;

    RefreshItemSlot();
}

void UItemSlotWidget::RefreshItemSlot()
{
    if (!TargetItemInventory.IsValid() || ItemID.IsNone() || Quantity <= 0)
    {
        ClearItemSlot();
        return;
    }

    FItemData* ItemInfo = TargetItemInventory->GetItemInfo(ItemID);
    if (!ItemInfo)
    {
        ClearItemSlot();
        return;
    }

    UTexture2D* Tex = ItemInfo->Icon.LoadSynchronous();
    ItemIconImage->SetBrushFromTexture(Tex);
    ItemIconImage->SetBrushTintColor(FLinearColor::White);

    CountText->SetText(FText::AsNumber(Quantity));
    CountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UItemSlotWidget::ClearItemSlot()
{
    if (ItemIconImage)
    {
        ItemIconImage->SetBrushFromTexture(nullptr);
        ItemIconImage->SetBrushTintColor(FLinearColor::Transparent);
    }

    if (CountText)
    {
        CountText->SetVisibility(ESlateVisibility::Hidden);
    }
}
