// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryWidget.h"
#include "UI/Inventory/ItemSlotWidget.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"

#include "Components/InventoryComponent.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//if(CloseButton)
	//{
	//	CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnCloseInventoryClicked);
	//}
	if (ItemSlotGridPanel)
	{
		int32 size = ItemSlotGridPanel->GetChildrenCount();
		for(int32 i = 0; i < size; i++)
		{
			UItemSlotWidget* ItmeSlotWidget = Cast<UItemSlotWidget>(ItemSlotGridPanel->GetChildAt(i)); //특정위젯을 가져올수있음
			//ItemSlotWidget->InitializeItemSlot(i, -1); //초기화
		}
	}
}


void UInventoryWidget::InitializeInventoryWidget(UInventoryComponent* InventoryComponent)
{
	if (InventoryComponent)
	{
		TargetInventory = InventoryComponent;
		if (TargetInventory.IsValid())
		{
			//int 32 size = FMath::Min(ItemSlotGridPanel->GetChildrenCount(), TargetInventory->GetGeneralItemSlots().Num());
			//UE_LOG(LogTemp, Warning, TEXT("인벤토리 위젯 초기화"));
			//for (int i = 0; i < TargetInventory->GetItemsCount(); i++)
			//{
			//	TargetInventory->GetItemInfo(i);
			//}
		}
	}
}

void UInventoryWidget::ClearInventoryWidget()
{
	TargetInventory = nullptr;
}

void UInventoryWidget::OnCloseInventoryClicked()
{
	OnInventoryCloseRequested.Broadcast();
}
