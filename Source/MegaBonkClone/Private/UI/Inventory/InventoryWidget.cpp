// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryWidget.h"
#include "Components/Button.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//if(CloseButton)
	//{
	//	CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnCloseInventoryClicked);
	//}
}

void UInventoryWidget::OnCloseInventoryClicked()
{
	OnInventoryCloseRequested.Broadcast();
}
