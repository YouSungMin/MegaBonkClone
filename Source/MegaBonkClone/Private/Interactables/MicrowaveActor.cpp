// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/MicrowaveActor.h"
#include "Components/InventoryComponent.h"
#include <Characters/Components/StatusComponent.h>

#include "Framework/MainHUD.h"

// Sets default values
AMicrowaveActor::AMicrowaveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	MicrowaveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MicrowaveMesh->SetupAttachment(Root);
	MicrowaveMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	MicrowaveMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AMicrowaveActor::StartCooking(FName SelectedItemID)
{
	if (CurrentState != EMicrowaveState::Ready) return;
	if (!Player.IsValid()) return;

	UStatusComponent* Status = Player.Get()->FindComponentByClass<UStatusComponent>();

	// [골드 차감] 실제 비용 지불 (StatusComponent의 SpendGold 활용)
	// SpendGold가 false를 반환하면(돈 부족) 중단
	if (Status && Status->SpendGold(GoldCost))
	{
		UE_LOG(LogTemp, Log, TEXT("%d 골드 사용, 조리 시작"), GoldCost);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("결제 실패: 골드가 부족합니다."));
		return;
	}

	// 1. 상태 변경 및 아이템 저장
	SavedItemID = SelectedItemID;
	CurrentState = EMicrowaveState::Cooking;

	if (!Inventory) return;
	// 2. [페널티] 선택한 아이템을 제외한 나머지 중 하나 랜덤 제거
	TArray<FName> CandidateIDs;
	const TArray<FInventorySlot>& CurrentSlots = Inventory->GetGeneralItemSlots();

	for (const FInventorySlot& Slot : CurrentSlots)
	{
		// 방금 받은(복사된) 아이템 종류는 삭제 대상에서 제외
		if (Slot.ItemID != SavedItemID)
		{
			CandidateIDs.Add(Slot.ItemID);
		}
	}

	if (CandidateIDs.Num() > 0)
	{
		int32 RandIdx = FMath::RandRange(0, CandidateIDs.Num() - 1);
		FName TargetDeleteID = CandidateIDs[RandIdx];

		// 인벤토리 컴포넌트에 구현한 RemoveItem 호출
		Inventory->RemoveItem(TargetDeleteID, 1);
		UE_LOG(LogTemp, Warning, TEXT("등가교환 발생! 사라진 아이템: %s"), *TargetDeleteID.ToString());

	}
	// 2. 타이머 시작 (지정된 시간 후 OnCookingFinished 호출)
	GetWorldTimerManager().SetTimer(CookingTimerHandle, this, &AMicrowaveActor::OnCookingFinished, CookTime, false);

	UE_LOG(LogTemp, Log, TEXT("전자레인지 가동 시작! (대상: %s, 소요시간: %.1f초)"), *SavedItemID.ToString(), CookTime);

	// (옵션) UI 닫기, 가동 사운드 재생 등
}

// Called when the game starts or when spawned
void AMicrowaveActor::BeginPlay()
{
	Super::BeginPlay();

	DetermineRarity();		// 등급 결정
	UpdateMeshTexture();	// 외형 적용
}
void AMicrowaveActor::DetermineRarity()
{
	// 데이터가 없으면 기본값 세팅 (방어 코드)
	if (RarityDropRates.Num() == 0)
	{
		RarityDropRates.Add(EItemGrade::Common, 60.0f);
		RarityDropRates.Add(EItemGrade::Rare, 30.0f);
		RarityDropRates.Add(EItemGrade::Epic, 10.0f);
		RarityDropRates.Add(EItemGrade::Legendary, 5.0f);
	}

	// 전체 가중치 합계 계산
	float TotalWeight = 0.0f;
	for (const auto& Entry : RarityDropRates)
	{
		TotalWeight += Entry.Value;
	}

	// 랜덤 값 뽑기 및 등급 결정
	float RandomValue = FMath::RandRange(0.0f, TotalWeight);
	float CurrentWeightSum = 0.0f;

	CurrentRarity = EItemGrade::Common; // Default fallback

	for (const auto& Entry : RarityDropRates)
	{
		CurrentWeightSum += Entry.Value;
		if (RandomValue <= CurrentWeightSum)
		{
			CurrentRarity = Entry.Key;
			break;
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("Microwave Rarity Decided: %d"), (int32)CurrentRarity);
}

void AMicrowaveActor::UpdateMeshTexture()
{
	if (!MicrowaveMesh) return;
	
	UMaterialInstanceDynamic* DMI = MicrowaveMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (!DMI) return;

	UTexture2D* SelectedTexture = nullptr;

	switch (CurrentRarity)
	{
	case EItemGrade::Epic:
		SelectedTexture = EpicTexture;
		break;

	case EItemGrade::Rare:
		SelectedTexture = RareTexture;
		break;

	case EItemGrade::Legendary:
		SelectedTexture = LegendaryTexture;
		break;

	case EItemGrade::Common:
	default:
		SelectedTexture = CommonTexture;
		break;
	}

	// 텍스처가 유효하다면 머티리얼 파라미터 변경
	if (SelectedTexture != nullptr)
	{
		DMI->SetTextureParameterValue(FName("DiffuseColorMap"), SelectedTexture);
	}

	// (디버깅용) 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("Microwave Rarity: %d"), (int32)CurrentRarity);
}

void AMicrowaveActor::OnCookingFinished()
{
	CurrentState = EMicrowaveState::Completed;
	UE_LOG(LogTemp, Log, TEXT("조리 완료! 상호작용하여 아이템을 꺼내세요."));

	// 소리 재생, 파티클 이펙트 등
}

void AMicrowaveActor::RetrieveResult()
{
	if (CurrentState != EMicrowaveState::Completed) return;
	if(!Player.IsValid()) return;
	if (!Inventory) return;

	// 1. [보상] 선택했던 아이템 1개 추가 (복사)
	Inventory->AddItem(SavedItemID, 1);
	UE_LOG(LogTemp, Log, TEXT("아이템 획득(복사): %s"), *SavedItemID.ToString());

	// 3. [초기화] 재사용 가능하도록 상태 리셋
	CurrentState = EMicrowaveState::Ready;
	SavedItemID = NAME_None;
}

void AMicrowaveActor::Interact_Implementation(AActor* PlayerActor)
{
	Player = PlayerActor;

	switch (CurrentState)
	{
	case EMicrowaveState::Ready:
	{
		// [골드 체크] 먼저 골드가 충분한지 확인
		UStatusComponent* Status = Player.Get()->FindComponentByClass<UStatusComponent>();
		if (!Status) return;

		// StatusComponent의 현재 골드 확인
		if (Status->GetCurrentGold() < GoldCost)
		{
			UE_LOG(LogTemp, Warning, TEXT("골드가 부족합니다! (필요: %d, 보유: %.0f)"), GoldCost, Status->GetCurrentGold());
			// 여기에 "골드가 부족합니다" 같은 UI 메시지를 띄우는 로직 추가 가능
			return;
		}

		// [인벤토리 체크] 복사할 아이템 목록 필터링
		Inventory = Player.Get()->FindComponentByClass<UInventoryComponent>();
		if (!Inventory) return;


		const TArray<FInventorySlot>& AllItems = Inventory->GetGeneralItemSlots();
		TArray<FMicrowaveSlotInfo> FilteredList;

		for (const FInventorySlot& Slot : AllItems)
		{
			FItemData* ItemInfo = Inventory->GetItemInfo(Slot.ItemID);
			if (ItemInfo && ItemInfo->Grade == CurrentRarity)
			{
				FMicrowaveSlotInfo NewInfo;
				NewInfo.ItemID = Slot.ItemID;
				NewInfo.ItemName = ItemInfo->Name;
				NewInfo.CurrentQuantity = Slot.Quantity;
				if (!ItemInfo->Icon.IsNull())
					NewInfo.Icon = ItemInfo->Icon.LoadSynchronous();
				UE_LOG(LogTemp, Log, TEXT("현재 캐릭터의 아이템 : %s"),*NewInfo.ItemName.ToString());
				FilteredList.Add(NewInfo);
			}
		}
		if (FilteredList.Num() < 2)
		{
			UE_LOG(LogTemp, Warning, TEXT("복사에 필요한 아이템이 부족합니다"), (int32)CurrentRarity);
			return;
		}

		if (FilteredList.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("등급(%d)에 맞는 아이템이 인벤토리에 없습니다."), (int32)CurrentRarity);
			return;
		}

		// 조건 만족 시 UI 오픈 (비용은 UI에서 선택 시 차감)
		//if (OnMicrowaveOpen.IsBound())
		//{
			//OnMicrowaveOpen.Broadcast(FilteredList, this);
		//}

		// E눌렀을때 HUD에 UI 오픈 요청
		if (APawn* PlayerPawn = Cast<APawn>(PlayerActor))
		{
			if (APlayerController* PC = Cast<APlayerController>(PlayerPawn->GetController()))
			{
				if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
				{
					HUD->ShowMicrowave(this, FilteredList);
				}
			}
		}
	}
	break;

	case EMicrowaveState::Cooking:
	{
		// 조리 중일 때 상호작용하면 남은 시간 알려줌
		float Remaining = GetWorldTimerManager().GetTimerRemaining(CookingTimerHandle);
		UE_LOG(LogTemp, Log, TEXT("전자레인지 가동 중... 남은 시간: %.1f초"), Remaining);
	}
	break;

	case EMicrowaveState::Completed:
	{
		// 조리가 끝났으므로 결과물 수령 시도
		RetrieveResult();
	}
	break;
	}
}

