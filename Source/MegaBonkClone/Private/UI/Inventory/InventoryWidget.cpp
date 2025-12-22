// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryWidget.h"
#include "UI/Inventory/ItemSlotWidget.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"

#include "Components/InventoryComponent.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemSlotWidgets.Reset();

	const int32 ChildCount = ItemSlotGridPanel->GetChildrenCount();
	for (int32 i = 0; i < ChildCount; ++i)
	{
		if (UItemSlotWidget* SlotWidget = Cast<UItemSlotWidget>(ItemSlotGridPanel->GetChildAt(i)))
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

void UInventoryWidget::InitializeInventoryWidget(UInventoryComponent* InventoryComponent)
{
	TargetInventory = InventoryComponent;

	if (!TargetInventory.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("타깃인벤토리 확인하기"));
		return;
	}

	// NativeConstruct보다 먼저 호출되는 경우 대비(안전장치)
	if (ItemSlotWidgets.Num() == 0 && ItemSlotGridPanel)
	{
		const int32 ChildCount = ItemSlotGridPanel->GetChildrenCount();
		for (int32 i = 0; i < ChildCount; ++i)
		{
			if (UItemSlotWidget* SlotWidget = Cast<UItemSlotWidget>(ItemSlotGridPanel->GetChildAt(i)))
			{
				ItemSlotWidgets.Add(SlotWidget);
			}
		}
	}
		//아이템 추가되면 갱신
		TargetInventory->OnItemAdd.AddDynamic(this, &UInventoryWidget::HandleItemAdded);

		RefreshInventoryWidget();
	}

void UInventoryWidget::RefreshInventoryWidget()
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

void UInventoryWidget::ClearInventoryWidget()
{
	if (TargetInventory.IsValid())
	{
		TargetInventory->OnItemAdd.RemoveDynamic(this, &UInventoryWidget::HandleItemAdded);
	}
	TargetInventory = nullptr;
}

void UInventoryWidget::HandleItemAdded(FName ItemID, const FItemData& ItemData)
{
	//전체리프레시
	RefreshInventoryWidget();
	UE_LOG(LogTemp, Warning, TEXT("델리게이트 발생"));
}
