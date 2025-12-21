// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHudWidget.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Characters/Components/StatusComponent.h"
#include "Components/InventoryComponent.h"
#include "UI/ResourceBarWidget.h"
#include "UI/Inventory/InventoryWidget.h"
#include "UI/PlayerStats/PlayerStatsWidget.h"

void UMainHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ClosePanels();	//처음에 패널들 닫기
	
	APlayerCharacter* player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (player)
	{
		if (UStatusComponent* status = player->GetStatusComponent())
		{
			//if (HealthBar.IsValid())
			//{
			//	HealthBar->RefreshWidget(resource->GetCurrentHP(), resource->GetResultMaxHP());
			//}
			//ShieldBar->RefreshWidget(resource->)
		}

		//if (UInventoryComponent* inventoryComponent = player->GetInventoryComponent())
		//{
		//	Inventory->OnInventoryCloseRequested.AddDynamic(this, &UMainHudWidget::CloseInventory);

		//	// 인벤토리컴포넌트의 내용을 바탕으로 InventoryWidget 채우기
		//}
	}
}

void UMainHudWidget::OpenPanels()
{
	InventoryPanel->SetVisibility(ESlateVisibility::Visible);	//인벤토리 패널 보이기
	PlayerStatsPanel->SetVisibility(ESlateVisibility::Visible);	//플레이어스탯 패널 보이기
}

void UMainHudWidget::ClosePanels()
{
	InventoryPanel->SetVisibility(ESlateVisibility::Hidden);	//인벤토리 패널 숨기기
	PlayerStatsPanel->SetVisibility(ESlateVisibility::Hidden);	//플레이어스탯 패널 숨기기
}

