// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameWeaponBarWidget.h"
#include "UI/Inventory/WeaponSlotWidget.h"
#include "Components/UniformGridPanel.h"

#include "Data/WeaponDataStructs.h"
#include "Characters/Components/WeaponSystemComponent.h"

void UInGameWeaponBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponSlotWidgets.Reset();

	const int32 ChildCount = WeaponSlotGridPanel->GetChildrenCount();
	for (int32 i = 0; i < ChildCount; ++i)
	{
		if (UWeaponSlotWidget* SlotWidget = Cast<UWeaponSlotWidget>(WeaponSlotGridPanel->GetChildAt(i)))
		{
			WeaponSlotWidgets.Add(SlotWidget);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WeaponBar %d (GridPanel 자식 확인)"), i);
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("InventoryWidget: Cached SlotWidgets = %d / GridChildren = %d"), i);
}

void UInGameWeaponBarWidget::InitializeInventoryWidget(UWeaponSystemComponent* WeaponSystemComponent)
{
	TargetWeaponInventory = WeaponSystemComponent;

	if (!TargetWeaponInventory.IsValid())
	{
		//UE_LOG(LogTemp, Warning, TEXT("타깃인벤토리 확인하기"));
		return;
	}

	// NativeConstruct보다 먼저 호출되는 경우 대비(안전장치)
	if (WeaponSlotWidgets.Num() == 0 && WeaponSlotGridPanel)
	{
		const int32 ChildCount = WeaponSlotGridPanel->GetChildrenCount();
		for (int32 i = 0; i < ChildCount; ++i)
		{
			if (UWeaponSlotWidget* SlotWidget = Cast<UWeaponSlotWidget>(WeaponSlotGridPanel->GetChildAt(i)))
			{
				WeaponSlotWidgets.Add(SlotWidget);
			}
		}
	}
	//아이템 추가되면 갱신

	TargetWeaponInventory->OnWeaponChanged.RemoveDynamic(this, &UInGameWeaponBarWidget::HandleWeaponChanged);
	TargetWeaponInventory->OnWeaponChanged.AddDynamic(this, &UInGameWeaponBarWidget::HandleWeaponChanged);

	RefreshInventoryWidget();
}


void UInGameWeaponBarWidget::RefreshInventoryWidget()
{
	if (!TargetWeaponInventory.IsValid())
		return;

	// 1) 전부 비우기
	for (UWeaponSlotWidget* SlotWidget : WeaponSlotWidgets)
	{
		if (SlotWidget)
		{
			SlotWidget->InitializeWeaponSlot(NAME_None, 0, TargetWeaponInventory.Get());
		}
	}

	// 2) 무기 데이터 채우기 (3칸)
	const TArray<FWeaponSlot>& Slots = TargetWeaponInventory->GetWeaponSlots();

	const int32 Count = FMath::Min(Slots.Num(), WeaponSlotWidgets.Num());
	for (int32 i = 0; i < Count; ++i)
	{
		const FWeaponSlot& S = Slots[i];
		if (S.WeaponID.IsNone() || S.Level <= 0)
			continue;

		WeaponSlotWidgets[i]->InitializeWeaponSlot(S.WeaponID, S.Level, TargetWeaponInventory.Get());
	}
}

void UInGameWeaponBarWidget::ClearInventoryWidget()
{
	if (TargetWeaponInventory.IsValid())
	{
		TargetWeaponInventory->OnWeaponChanged.RemoveDynamic(this, &UInGameWeaponBarWidget::HandleWeaponChanged);
	}
	TargetWeaponInventory = nullptr;
}

void UInGameWeaponBarWidget::HandleWeaponChanged()
{
	//전체리프레시
	RefreshInventoryWidget();
	UE_LOG(LogTemp, Warning, TEXT("델리게이트 발생"));

}
