// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/WeaponSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Data/WeaponDataStructs.h"
#include "Characters/Components/WeaponSystemComponent.h"

void UWeaponSlotWidget::InitializeWeaponSlot(FName InWeaponID, int32 InLevel, UWeaponSystemComponent* InWeapon)
{
    WeaponID = InWeaponID;
    Level = InLevel;
    TargetWeaponInventory = InWeapon;

    RefreshWeaponSlot();
}

void UWeaponSlotWidget::RefreshWeaponSlot()
{

    UE_LOG(LogTemp, Warning, TEXT("[WeaponSlot] Refresh start ID=%s Lv=%d ValidInv=%d"),
        *WeaponID.ToString(), Level, TargetWeaponInventory.IsValid());

    if (!TargetWeaponInventory.IsValid() || WeaponID.IsNone() || Level <= 0)
    {
        ClearWeaponSlot();
        return;
    }

    FWeaponData* WeaponInfo = TargetWeaponInventory->GetWeaponInfo(WeaponID);
    if (!WeaponInfo)
    {
        ClearWeaponSlot();
        return;
    }

    UTexture2D* Tex = WeaponInfo->Icon.LoadSynchronous();
    WeaponIconImage->SetBrushFromTexture(Tex);
    WeaponIconImage->SetBrushTintColor(FLinearColor::White);

    LevelText->SetText(FText::FromString(FString::Printf(TEXT("LV%d"), Level)));
    //LevelText->SetText(FText::AsNumber(Level));
    LevelText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    
}

void UWeaponSlotWidget::ClearWeaponSlot()
{
    if (WeaponIconImage)
    {
        WeaponIconImage->SetBrushFromTexture(nullptr);
        WeaponIconImage->SetBrushTintColor(FLinearColor::Transparent);
    }

    if (LevelText)
    {
        LevelText->SetVisibility(ESlateVisibility::Hidden);
    }
}
