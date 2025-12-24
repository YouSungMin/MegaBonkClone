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
			UE_LOG(LogTemp, Warning, TEXT("WeaponBar %d (GridPanel �ڽ� Ȯ��)"), i);
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("InventoryWidget: Cached SlotWidgets = %d / GridChildren = %d"), i);
}

void UInGameWeaponBarWidget::InitializeInventoryWidget(UWeaponSystemComponent* WeaponSystemComponent)
{
	TargetWeaponInventory = WeaponSystemComponent;

	if (!TargetWeaponInventory.IsValid())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Ÿ���κ��丮 Ȯ���ϱ�"));
		return;
	}

	// NativeConstruct���� ���� ȣ��Ǵ� ��� ���(������ġ)
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
	//������ �߰��Ǹ� ����

	TargetWeaponInventory->OnWeaponChanged.RemoveDynamic(this, &UInGameWeaponBarWidget::HandleWeaponChanged);
	TargetWeaponInventory->OnWeaponChanged.AddDynamic(this, &UInGameWeaponBarWidget::HandleWeaponChanged);

	RefreshInventoryWidget();


	UE_LOG(LogTemp, Warning, TEXT("[WeaponBar] Grid=%s CachedWidgets=%d"),
		WeaponSlotGridPanel ? TEXT("OK") : TEXT("NULL"),
		WeaponSlotWidgets.Num());

}


void UInGameWeaponBarWidget::RefreshInventoryWidget()
{

	UE_LOG(LogTemp, Warning, TEXT("[WeaponBar] Refresh called"));


	if (!TargetWeaponInventory.IsValid())
		return;

	// 1) ���� ����
	for (UWeaponSlotWidget* SlotWidget : WeaponSlotWidgets)
	{
		if (SlotWidget)
		{
			SlotWidget->InitializeWeaponSlot(NAME_None, 0, TargetWeaponInventory.Get());
		}
	}

	// 2) ���� ������ ä��� (3ĭ)
	const TArray<FWeaponSlot>& Slots = TargetWeaponInventory->GetWeaponSlots();

	const int32 Count = FMath::Min(Slots.Num(), WeaponSlotWidgets.Num());
	for (int32 i = 0; i < Count; ++i)
	{
		const FWeaponSlot& S = Slots[i];
		if (S.WeaponID.IsNone() || S.Level <= 0)
			continue;

		WeaponSlotWidgets[i]->InitializeWeaponSlot(S.WeaponID, S.Level, TargetWeaponInventory.Get());
	
		UE_LOG(LogTemp, Warning, TEXT("[WeaponBar] Slot[%d] ID=%s Lv=%d"),
			i, *Slots[i].WeaponID.ToString(), Slots[i].Level);

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
	UE_LOG(LogTemp, Warning, TEXT("[WeaponBar] HandleWeaponChanged() CALLED"));

	//��ü��������
	RefreshInventoryWidget();
	UE_LOG(LogTemp, Warning, TEXT("��������Ʈ �߻�"));

}
