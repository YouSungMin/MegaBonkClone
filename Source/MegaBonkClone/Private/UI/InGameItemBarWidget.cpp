// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameItemBarWidget.h"
#include "UI/Inventory/ItemSlotWidget.h"

#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Components/InventoryComponent.h"

void UInGameItemBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ItemWrapBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemWrapBox is NuLL"));
		return;
	}
	ItemSlotWidgets.Reset();

	const int32 ChildCount = ItemWrapBox->GetChildrenCount();
	for (int32 i = 0; i < ChildCount; ++i)
	{
		if (UItemSlotWidget* SlotWidget = Cast<UItemSlotWidget>(ItemWrapBox->GetChildAt(i)))
		{
			ItemSlotWidgets.Add(SlotWidget);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventor %d (GridPanel 자식 확인)"), i);
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("InventoryWidget: Cached SlotWidgets = %d / GridChildren = %d"), i);
}

void UInGameItemBarWidget::InitializeInventoryWidget(UInventoryComponent* InventoryComponent)
{
	TargetInventory = InventoryComponent;

	if (!TargetInventory.IsValid())
	{
		//UE_LOG(LogTemp, Log, TEXT("타깃인벤토리 확인하기"));
		return;
	}

	// NativeConstruct보다 먼저 호출되는 경우 대비(안전장치)
	if (ItemSlotWidgets.Num() == 0 && ItemWrapBox)
	{
		const int32 ChildCount = ItemWrapBox->GetChildrenCount();
		for (int32 i = 0; i < ChildCount; ++i)
		{
			if (UItemSlotWidget* SlotWidget = Cast<UItemSlotWidget>(ItemWrapBox->GetChildAt(i)))
			{
				ItemSlotWidgets.Add(SlotWidget);
			}
		}
	}
	//아이템 추가되면 갱신
	TargetInventory->OnItemAdd.RemoveDynamic(this, &UInGameItemBarWidget::HandleItemAdded);
	TargetInventory->OnItemAdd.AddDynamic(this, &UInGameItemBarWidget::HandleItemAdded);

	RefreshInventoryWidget();
}

void UInGameItemBarWidget::RefreshInventoryWidget()
{
	if (!TargetInventory.IsValid())
		return;

	// 1) 8칸 전부 비우기
	for (UItemSlotWidget* SlotWidget : ItemSlotWidgets)
	{
		if (SlotWidget)
		{
			SlotWidget->InitializeItemSlot(NAME_None, 0, TargetInventory.Get());
		}
	}

	// 2) 인벤 데이터로 채우기 (앞에서부터)
	const TArray<FInventorySlot>& Items = TargetInventory->GetGeneralItemSlots();

	int32 VisibleIndex = 0;
	for (const FInventorySlot& InvSlot : Items)
	{
		if (InvSlot.ItemID.IsNone() || InvSlot.Quantity <= 0)
			continue;

		if (!ItemSlotWidgets.IsValidIndex(VisibleIndex))
			break;

		ItemSlotWidgets[VisibleIndex]->InitializeItemSlot(InvSlot.ItemID, InvSlot.Quantity, TargetInventory.Get());
		VisibleIndex++;
	}

}

void UInGameItemBarWidget::ClearInventoryWidget()
{
	if (TargetInventory.IsValid())
	{
		TargetInventory->OnItemAdd.RemoveDynamic(this, &UInGameItemBarWidget::HandleItemAdded);
	}
	TargetInventory = nullptr;
}

void UInGameItemBarWidget::HandleItemAdded(FName ItemID, const FItemData& ItemData)
{
	//전체리프레시
	RefreshInventoryWidget();
	UE_LOG(LogTemp, Warning, TEXT("델리게이트 발생"));
}
