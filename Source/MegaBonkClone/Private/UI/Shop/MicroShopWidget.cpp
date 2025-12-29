// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/MicroShopWidget.h"
#include "UI/Shop/MicroSlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Button.h"
#include "Framework/MainHUD.h"
//#include "GameFramework/PlayerController.h"

void UMicroShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ExitButton)
	{
		ExitButton->OnClicked.RemoveDynamic(this, &UMicroShopWidget::OnExitClicked);
		ExitButton->OnClicked.AddDynamic(this, &UMicroShopWidget::OnExitClicked);
	}
}

void UMicroShopWidget::NativeDestruct()
{
	TeardownMicrowave();
	Super::NativeDestruct();
}

//전자레인지 액터와 아이템 목록으로 초기화
void UMicroShopWidget::InitWithMicrowave(AMicrowaveActor* InMicrowave, const TArray<FMicrowaveSlotInfo>& InList)
{
	TeardownMicrowave();

	MicrowaveRef = InMicrowave;
	CachedList = InList;

	if (MicrowaveRef)
	{
		MicrowaveRef->OnInventoryUpdated.RemoveDynamic(this, &UMicroShopWidget::HandleInventoryUpdated);
		MicrowaveRef->OnInventoryUpdated.AddDynamic(this, &UMicroShopWidget::HandleInventoryUpdated);
	}

	RebuildGrid();
}

//CachedList를 기반으로 그리드 재구성
void UMicroShopWidget::RebuildGrid()
{
	if (!Grid) return;
	Grid->ClearChildren();

	if (!SlotWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[MicroShop] SlotWidgetClass is null"));
		return;
	}

	//아이템 개수만큼 슬롯 위젯 생성해서 Grid에 추가
	for (int32 i = 0; i < CachedList.Num(); i++)
	{
		UMicroSlotWidget* SlotW = CreateWidget<UMicroSlotWidget>(GetOwningPlayer(), SlotWidgetClass);
		if (!SlotW) continue;

		SlotW->SetSlotInfo(CachedList[i]);

		SlotW->OnSlotClicked.RemoveDynamic(this, &UMicroShopWidget::OnSlotPicked);
		SlotW->OnSlotClicked.AddDynamic(this, &UMicroShopWidget::OnSlotPicked);

		const int32 Row = i / Columns;
		const int32 Col = i % Columns;

		// 그리드에 추가
		if (UUniformGridSlot* GridSlot = Grid->AddChildToUniformGrid(SlotW, Row, Col))
		{
			GridSlot->SetHorizontalAlignment(HAlign_Fill);
			GridSlot->SetVerticalAlignment(VAlign_Fill);
		}
	}
}

void UMicroShopWidget::OnSlotPicked(FName ItemID)
{
	if (!MicrowaveRef) return;

	UE_LOG(LogTemp, Warning, TEXT("[MicroShop] Picked ItemID = %s"), *ItemID.ToString());

	// 전자레인지 로직 실행 -> 선택한 아이템으로 조리 시작
	MicrowaveRef->StartCooking(ItemID);

	// UI 닫기
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
		{
			HUD->CloseCenterUI();
		}
	}
}

//나가기 버튼 클릭했을때 호출되는 함수
void UMicroShopWidget::OnExitClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("[MicroShop] Exit Clicked"));

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
		{
			HUD->CloseCenterUI();
		}
	}
}

void UMicroShopWidget::TeardownMicrowave()
{
	if (MicrowaveRef)
	{
		MicrowaveRef->OnInventoryUpdated.RemoveDynamic(this, &UMicroShopWidget::HandleInventoryUpdated);
	}
}

void UMicroShopWidget::HandleInventoryUpdated()
{
	OnExitClicked();
}