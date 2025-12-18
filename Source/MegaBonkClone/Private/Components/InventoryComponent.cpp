// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// 초기화
	SecretBookSlots.Empty();
	GeneralItems.Empty();
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UInventoryComponent::AddSecretBook(FName BookID)
{
    FItemData* FoundData = GetItemInfo(BookID);
    if (!FoundData)
    {
        UE_LOG(LogTemp, Warning, TEXT("AddSecretBook Failed: Invalid ID [%s]"), *BookID.ToString());
        return false;
    }


    //이미 보유 중이면 레벨업
    for (FInventorySlot& Slot : SecretBookSlots)
    {
        if (Slot.ItemID == BookID)
        {
            Slot.Level++;
            UE_LOG(LogTemp, Log, TEXT("SecretBook Level Up: %s (Lv.%d)"), *BookID.ToString(), Slot.Level);
            OnItemAdd.Broadcast(BookID, *FoundData);
            return true;
        }
    }

    // 슬롯 제한 확인
    if (SecretBookSlots.Num() >= MaxSecretBookCount)
    {
        UE_LOG(LogTemp, Warning, TEXT("AddSecretBook Failed: Slots Full"));
        return false;
    }

    // 신규 추가
    FInventorySlot NewSlot;
    NewSlot.ItemID = BookID;
    NewSlot.Level = 1;
    NewSlot.Quantity = 1;

    SecretBookSlots.Add(NewSlot);
    OnItemAdd.Broadcast(BookID, *FoundData);
    UE_LOG(LogTemp, Log, TEXT("SecretBook Added: %s"), *BookID.ToString());

    return true;
}

void UInventoryComponent::AddItem(FName ItemID, int32 Count)
{
    if (Count <= 0) return;

    FItemData* FoundData = GetItemInfo(ItemID);
    if (!FoundData)
    {
        UE_LOG(LogTemp, Warning, TEXT("아이템 추가 실패: Invalid Item ID [%s]"), *ItemID.ToString());
        return;
    }


    // 이미 인벤토리에 있는지 확인 
    for (FInventorySlot& Slot : GeneralItems)
    {
        if (Slot.ItemID == ItemID)
        {
            Slot.Quantity += Count;
            UE_LOG(LogTemp, Log, TEXT("Item 갯수 추가: %s (Qty: %d)"), *ItemID.ToString(), Slot.Quantity);
            OnItemAdd.Broadcast(ItemID, *FoundData);
            return;
        }
    }

    // 없으면 새로 추가
    FInventorySlot NewSlot;
    NewSlot.ItemID = ItemID;
    NewSlot.Level = 1; // 일반 아이템은 레벨 의미 없음
    NewSlot.Quantity = Count;

    GeneralItems.Add(NewSlot);
    OnItemAdd.Broadcast(ItemID, *FoundData);
    UE_LOG(LogTemp, Log, TEXT("Item 추가: %s (Qty: %d)"), *ItemID.ToString(), Count);
}


FItemData* UInventoryComponent::GetItemInfo(FName ItemID) const
{
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemDataTable is not set in InventoryComponent!"));
		return nullptr;
	}

	static const FString ContextString(TEXT("GetItemInfo"));

	return ItemDataTable->FindRow<FItemData>(ItemID, ContextString);
}



