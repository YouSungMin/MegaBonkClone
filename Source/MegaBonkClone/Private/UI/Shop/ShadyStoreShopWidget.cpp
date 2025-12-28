// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShadyStoreShopWidget.h"
#include "Interactables/ShadyGuyActor.h"

void UShadyStoreShopWidget::InitWithShadyGuy(AShadyGuyActor* InGuy, const TArray<struct FShopSlotInfo>& Items)
{
    UE_LOG(LogTemp, Warning, TEXT("[ShadyStore] InitWithShadyGuy called. Items=%d"), Items.Num());

    for (int32 i = 0; i < Items.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("  Slot%d: ID=%s Price=%d"),
            i,
            *Items[i].ItemID.ToString(),
            Items[i].Price
        );
    }
}
