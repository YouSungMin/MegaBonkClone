// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHudWidget.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Characters/Components/StatusComponent.h"
#include "Components/InventoryComponent.h"
#include "UI/ResourceBarWidget.h"


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

			// 인벤토리컴포넌트의 내용을 바탕으로 InventoryWidget 채우기
		//}
	}
}


void UMainHudWidget::OpenPanels()
{
	InventoryPanel->RefreshInventoryWidget();

	InventoryPanel->SetVisibility(ESlateVisibility::Visible);	//인벤토리 패널 보이기
	PlayerStatsPanel->SetVisibility(ESlateVisibility::Visible);	//플레이어스탯 패널 보이기

	OpenState = EOpenState::Open;	//열림 상태로 변경
}

void UMainHudWidget::ClosePanels()
{
	OpenState = EOpenState::Close;	//닫힘 상태로 변경	
	InventoryPanel->SetVisibility(ESlateVisibility::Hidden);	//인벤토리 패널 숨기기
	PlayerStatsPanel->SetVisibility(ESlateVisibility::Hidden);	//플레이어스탯 패널 숨기기

}

