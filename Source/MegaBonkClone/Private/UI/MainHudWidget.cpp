// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHudWidget.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Characters/Components/StatusComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/NamedSlot.h"
#include "Characters/Components/WeaponSystemComponent.h"
#include "Components/InventoryComponent.h"	
#include "Data/ItemDataStructs.h"
#include "Components/UniformGridPanel.h"
#include "UI/Inventory/SecretBookSlotWidget.h"
#include "UI/ResourceBarWidget.h"


void UMainHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ClosePanels();	//처음에 패널들 닫기
	
	APlayerCharacter* player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (player)
	{

		if (UInventoryComponent* inventoryComponent = player->GetInventoryComponent())
		{
			// 인벤토리컴포넌트의 내용을 바탕으로 InventoryWidget 채우기
			
			CachedInventory = inventoryComponent;

			//항상 떠있는 하단 아이템바
			if (ItemBar)
			{
				ItemBar->InitializeInventoryWidget(inventoryComponent);  // 너가 만든 함수명 그대로
			}
			if (InventoryPanel)
			{
				InventoryPanel->InitializeInventoryWidget(inventoryComponent);
			}

			// 1. 비전시 WBP에 배치된 시크릿 슬롯들 캐싱
			CacheSecretSlots();

			// 2. 비전시 처음 한 번 채우기
			RefreshSecretSlots();

			// 3. 인벤 변경 시 HUD 시크릿도 갱신
			CachedInventory->OnItemAdd.RemoveDynamic(this, &UMainHudWidget::HandleInventoryChanged);
			CachedInventory->OnItemAdd.AddDynamic(this, &UMainHudWidget::HandleInventoryChanged);
		}


		}
		if(UWeaponSystemComponent* weaponSystemComponent = player->FindComponentByClass<UWeaponSystemComponent>())
		{
			if (WeaponBar)
			{
				WeaponBar->InitializeInventoryWidget(weaponSystemComponent);
			}
		}

		if (UStatusComponent* status = player->GetStatusComponent())
		{
			//[바인딩] 값이 변하면 이 함수들을 실행해라!
			status->OnHPChanged.AddDynamic(this, &UMainHudWidget::HandleHPChanged);
			status->OnShieldChanged.AddDynamic(this, &UMainHudWidget::HandleShieldChanged);

			//[초기값 강제 실행] 바인딩 직후, 현재 상태로 한 번 그려줌 (타이밍 문제 해결)
			HandleHPChanged(status->GetCurrentHP(), status->GetResultMaxHP());
			HandleShieldChanged(status->GetResultShield(), status->GetResultShield());
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

void UMainHudWidget::SetCenterContent(UUserWidget* NewWidget)
{
	if (!CenterSlot) return;

	// 1. 기존 내용물 비우기
	CenterSlot->ClearChildren();

	// 2. 새 위젯이 있다면 슬롯에 넣기
	if (NewWidget)
	{
		CenterSlot->SetContent(NewWidget);
	}
}

void UMainHudWidget::ClearCenterContent()
{
	if (CenterSlot)
	{
		CenterSlot->ClearChildren();
	}
}

//비전시 슬롯 캐싱
void UMainHudWidget::CacheSecretSlots()
{
	SecretSlotWidgets.Reset();

	if (!SecretSlotGridPanel) return;

	for (int32 i = 0; i < SecretSlotGridPanel->GetChildrenCount(); ++i)
	{
		if (USecretBookSlotWidget* SecretSlot = Cast<USecretBookSlotWidget>(SecretSlotGridPanel->GetChildAt(i)))
		{
			SecretSlotWidgets.Add(SecretSlot);
		}
	}
}

void UMainHudWidget::RefreshSecretSlots()
{
	//1. 전부 비우기
	for (USecretBookSlotWidget* SecretSlot : SecretSlotWidgets)
	{
		if (SecretSlot) SecretSlot->InitializeSecretBookSlot(NAME_None, 0, CachedInventory);
	}

	//2. 데이터 있는 것만 순서대로 채우기
	const TArray<FInventorySlot>& Secrets = CachedInventory->GetSecretBookSlots();

	int32 idx = 0;
	for (const FInventorySlot& S : Secrets)
	{
		if (idx >= SecretSlotWidgets.Num()) break;
		if (S.ItemID.IsNone() || S.Level <= 0) continue;

		SecretSlotWidgets[idx++]->InitializeSecretBookSlot(S.ItemID, S.Level, CachedInventory);
	}
}

void UMainHudWidget::HandleInventoryChanged(FName ItemID, const FItemData& ItemData)
{
	RefreshSecretSlots();
}

void UMainHudWidget::HandleHPChanged(float CurrentHP, float MaxHP)
{
	if (HealthBar)
	{
		HealthBar->RefreshWidget(CurrentHP, MaxHP);
	}
}

void UMainHudWidget::HandleShieldChanged(float CurrentShield, float MaxShield)
{
	if (ShieldBar)
	{
		// 쉴드는 Max를 MaxShield로 쓸지, MaxHP로 쓸지 기획에 따라 결정 (여기선 MaxHP 기준)
		ShieldBar->RefreshWidget(CurrentShield, MaxShield);
	}
}