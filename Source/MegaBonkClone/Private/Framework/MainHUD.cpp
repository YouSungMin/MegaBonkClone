// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MainHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Shop/RechargeShopWidget.h"
#include "UI/Shop/MicroShopWidget.h"
#include "UI/Shop/ShadyStoreShopWidget.h"
#include "UI/Shop/UpgradeShopWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Characters/PlayAbleCharacter/PlayAbleCharacterController.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	
	if (MainWidgetClass)
	{
		MainWidgetInstance = CreateWidget<UMainHudWidget>(GetWorld(), MainWidgetClass);

		if (MainWidgetInstance)
		{
			MainWidgetInstance->AddToViewport();

			//HUD는 컨트롤러가 소유하므로, 컨트롤러에 메인위젯을 설정
			APlayAbleCharacterController* pc = Cast<APlayAbleCharacterController>(GetOwningPlayerController());
			if (pc)
			{
				pc->InitializeMainHudWidget(MainWidgetInstance);
			}
		}
	}
	

	TArray<AActor*> FoundActors;

	//레퍼런스 가져오기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		// 이름으로 찾기 (혹은 태그로 찾는 게 더 안전합니다)
		if (Actor && Actor->ActorHasTag(TEXT("ChestStudio")))
		{
			CachedStudioActor = Actor;
			//UE_LOG(LogTemp, Warning, TEXT("촬영용 스튜디오 액터 찾음: %s"), *Actor->GetName());
			break; 
		}
	}
}

void AMainHUD::ShowChestReward(const FItemData& ItemData)
{
	// 1. 위젯 인스턴스가 유효하지 않다면 새로 생성
	if (!ChestOpenWidget || !ChestOpenWidget->IsValidLowLevel())
	{
		if (ChestOpenClass)
		{
			ChestOpenWidget = CreateWidget<UChestOpen>(GetOwningPlayerController(), ChestOpenClass);

			if (ChestOpenWidget)
			{
				ChestOpenWidget->AddToViewport();

				// 촬영용 스튜디오 액터 연결 (최초 1회만 필요)
				if (CachedStudioActor)
				{
					ChestOpenWidget->SetStudioActor(CachedStudioActor);
				}
			}
		}
	}

	//메인 위젯의 중앙 슬롯에 넣음
	if (ChestOpenWidget)
	{
		// 메인 위젯의 함수 호출 -> 중앙 슬롯에 상자 UI 끼워넣기
		MainWidgetInstance->SetCenterContent(ChestOpenWidget);
		//능력치 인벤토리 패널 키기
		MainWidgetInstance->OpenPanels();
		// 연출 시작
		ChestOpenWidget->PlayLootSequence(ItemData);

		// 입력 모드 변경 (마우스 필요 시)
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			PC->SetShowMouseCursor(true);
			PC->SetInputMode(FInputModeUIOnly());

			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001f);
		}
	}
}

void AMainHUD::ShowRechargeSanctuary(ARechargeSanctuary* Sanctuary)
{
	if (!Sanctuary) return;

	//1.RechargePanelClass로 위젯 생성 + centerSlot에 끼우기
	OpenCenterPanel(RechargePanelClass, RechargeWidget);

	//2.URechargePanelClass로 캐스트해서 Sanctuary바인딩
	if (URechargeShopWidget* Shop = Cast<URechargeShopWidget>(RechargeWidget))
	{
		Shop->InitWithSanctuary(Sanctuary);
	}

	//3. 인벤토리/스탯 패널 열기
	MainWidgetInstance->OpenPanels();

	//4. 입력 모드 변경 (마우스 필요 시)
	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeUIOnly());

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001f);
	}
}

void AMainHUD::ShowShadyStore(class AShadyGuyActor* ShadyGuy, const TArray<struct FShopSlotInfo>& Items)
{
	if (!ShadyGuy) return;

	//1.RechargePanelClass로 위젯 생성 + centerSlot에 끼우기
	OpenCenterPanel(StorePanelClass, StoreWidget);

	//2.URechargePanelClass로 캐스트해서 Sanctuary바인딩
	if (UShadyStoreShopWidget* Shop = Cast<UShadyStoreShopWidget>(StoreWidget))
	{
		Shop->InitWithShadyGuy(ShadyGuy, Items);
	}

	//3. 인벤토리/스탯 패널 열기
	MainWidgetInstance->OpenPanels();

	//4. 입력 모드 변경 (마우스 필요 시)
	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeUIOnly());

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001f);
	}
}


void AMainHUD::ShowMicrowave(AMicrowaveActor* Microwave, const TArray<FMicrowaveSlotInfo>& FilteredList)
{
	if (!Microwave) return;

	//1.RechargePanelClass로 위젯 생성 + centerSlot에 끼우기
	OpenCenterPanel(MicroWavePanelClass, MicroWaveWidget);

	//2.URechargePanelClass로 캐스트해서 Sanctuary바인딩
	if (UMicroShopWidget* Shop = Cast<UMicroShopWidget>(MicroWaveWidget))
	{
		Shop->InitWithMicrowave(Microwave, FilteredList);
	}

	//3. 인벤토리/스탯 패널 열기
	MainWidgetInstance->OpenPanels();

	//4. 입력 모드 변경 (마우스 필요 시)
	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeUIOnly());

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001f);
	}
}

void AMainHUD::ShowUpgrade(URewardSystemComponent* RewardComp, const TArray<struct FRewardOption>& Options)
{
	if (!RewardComp) return;

	OpenCenterPanel(UpgradePanelClass, UpgradeWidget);

	if (UUpgradeShopWidget* Shop = Cast<UUpgradeShopWidget>(UpgradeWidget))
	{
		Shop->InitWithRewards(RewardComp, Options);
	}

	MainWidgetInstance->OpenPanels();

	if (APlayerController * PC = GetOwningPlayerController())
	{
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeUIOnly());

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001f);
	}
}

void AMainHUD::OpenCenterPanel(TSubclassOf<UUserWidget> PanelClass, TObjectPtr<UUserWidget>& WidgetInstance)
{
	// 1. 메인 위젯 유효성 검사
	if (!MainWidgetInstance) return;

	// 2. 위젯 인스턴스가 없으면 생성
	if (!WidgetInstance && PanelClass)
	{
		WidgetInstance = CreateWidget<UUserWidget>(GetOwningPlayerController(), PanelClass);
	}

	// 3. 중앙 슬롯에 끼워넣기
	if (WidgetInstance)
	{
		// 기존 내용 제거 후 새 위젯 부착
		MainWidgetInstance->SetCenterContent(WidgetInstance);

		//마우스 커서 보이게 설정
		if (APlayerController* PC = GetOwningPlayerController())
		{
			PC->SetShowMouseCursor(true);
			PC->SetInputMode(FInputModeUIOnly());
		}
	}
}

void AMainHUD::CloseCenterUI()
{
	if (MainWidgetInstance)
	{
		// 중앙 비우기
		MainWidgetInstance->ClearCenterContent();
		MainWidgetInstance->ClosePanels();
		// 입력 모드 게임으로 복귀
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			PC->SetShowMouseCursor(false);
			PC->SetInputMode(FInputModeGameOnly());

			UGameplayStatics::SetGlobalTimeDilation(GetWorld(),1.0f);
		}
	}
}

void AMainHUD::ShowGameOver()
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC || !GameOverWidgetClass) return;

	// 아직 없으면 생성
	if (!GameOverWidgetInstance)
	{
		GameOverWidgetInstance = CreateWidget<UUserWidget>(PC, GameOverWidgetClass);
	}

	// 화면에 올리기 (항상 맨 위)
	if (GameOverWidgetInstance && !GameOverWidgetInstance->IsInViewport())
	{
		GameOverWidgetInstance->AddToViewport(9999);
	}

	// UI 입력으로 전환 (버튼 클릭되게)
	PC->SetShowMouseCursor(true);
	PC->SetInputMode(FInputModeUIOnly());

	// 게임멈춤
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001f);
}

