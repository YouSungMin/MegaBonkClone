// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameItemBarWidget.h"
#include "UI/Inventory/ItemSlotWidget.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Components/InventoryComponent.h"
#include "Interactables/MicrowaveActor.h"
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
			UE_LOG(LogTemp, Warning, TEXT("Inventor %d (GridPanel �ڽ� Ȯ��)"), i);
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("InventoryWidget: Cached SlotWidgets = %d / GridChildren = %d"), i);
}

void UInGameItemBarWidget::InitializeInventoryWidget(UInventoryComponent* InventoryComponent)
{
	TargetInventory = InventoryComponent;

	if (!TargetInventory.IsValid())
	{
		//UE_LOG(LogTemp, Log, TEXT("Ÿ���κ��丮 Ȯ���ϱ�"));
		return;
	}

	// NativeConstruct���� ���� ȣ��Ǵ� ��� ���(������ġ)
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
	//������ �߰��Ǹ� ����
	TargetInventory->OnItemAdd.RemoveDynamic(this, &UInGameItemBarWidget::HandleItemAdded);
	TargetInventory->OnItemAdd.AddDynamic(this, &UInGameItemBarWidget::HandleItemAdded);

	TargetInventory->OnInventoryUpdated.RemoveDynamic(this, &UInGameItemBarWidget::HandleInventoryUpdated);
	TargetInventory->OnInventoryUpdated.AddDynamic(this, &UInGameItemBarWidget::HandleInventoryUpdated);

	RefreshInventoryWidget();
}

void UInGameItemBarWidget::RefreshInventoryWidget()
{
	if (!TargetInventory.IsValid())
		return;

	// 1) 8ĭ ���� ����
	for (UItemSlotWidget* SlotWidget : ItemSlotWidgets)
	{
		if (SlotWidget)
		{
			SlotWidget->InitializeItemSlot(NAME_None, 0, TargetInventory.Get());
		}
	}

	// 2) �κ� �����ͷ� ä��� (�տ�������)
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
	//��ü��������
	RefreshInventoryWidget();
	UE_LOG(LogTemp, Warning, TEXT("��������Ʈ �߻�"));
}

void UInGameItemBarWidget::HandleInventoryUpdated()
{
	RefreshInventoryWidget();
}


