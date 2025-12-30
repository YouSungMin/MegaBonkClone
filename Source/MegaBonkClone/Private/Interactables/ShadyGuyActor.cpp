// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/ShadyGuyActor.h"
#include "Interfaces/InventoryOwner.h"
#include "Framework/MainHUD.h"
#include "Data/ItemDataStructs.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
AShadyGuyActor::AShadyGuyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ShadyGuyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	ShadyGuyMesh->SetupAttachment(Root);
	ShadyGuyMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	ShadyGuyMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	ShadyGuyMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
}

void AShadyGuyActor::ProcessPurchase(int32 ItemIndex)
{
	if (!Player.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("구매자를 찾을 수 없습니다. (플레이어가 사망했거나 사라짐)"));
		return;
	}

	if (!Player->Implements<UInventoryOwner>())
	{
		UE_LOG(LogTemp, Error, TEXT("대상이 InventoryOwner 인터페이스를 구현하지 않았습니다."));
		return;
	}	

	// 인덱스 유효성 검사
	if (!CurrentShopItems.IsValidIndex(ItemIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("잘못된 아이템 인덱스입니다: %d"), ItemIndex);
		return;
	}

	// 선택된 아이템 정보 가져오기
	FShopSlotInfo SelectedSlot = CurrentShopItems[ItemIndex];
	FName SelectedItemID = SelectedSlot.ItemID;

	IInventoryOwner::Execute_ReceiveItem(Player.Get(), SelectedItemID, 1);
	UE_LOG(LogTemp, Log, TEXT("아이템 지급 완료: %s"), *SelectedItemID.ToString());
	bIsUsed = true;
	Destroy();

	// (옵션) UI를 닫거나 갱신하는 로직이 필요하면 여기에 델리게이트 호출 추가
}

void AShadyGuyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player.IsValid())
	{
		FVector StartLocation = GetActorLocation();
		FVector TargetLocation = Player->GetActorLocation();

		// 3. 바라봐야 할 목표 회전값 계산 (FindLookAtRotation)
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);

		// [중요] 적이 하늘/땅을 보지 않고 수평으로만 돌게 하려면 Pitch, Roll을 0으로 고정
		TargetRotation.Pitch = 0.0f;
		TargetRotation.Roll = 0.0f;

		// 4. 현재 회전값에서 목표 회전값으로 부드럽게 보간 (RInterpTo)
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f); // 5.0f는 회전 속도

		// 5. 적용
		SetActorRotation(NewRotation);
	}
}

// Called when the game starts or when spawned
void AShadyGuyActor::BeginPlay()
{
	Super::BeginPlay();

	DetermineRarity();		// 등급 결정
	UpdateMeshTexture();	// 외형 적용
	GenerateShopItems();	// 아이템 데이터 생성

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void AShadyGuyActor::Interact_Implementation(AActor* PlayerActor)
{
	if (!bIsUsed)
	{
		Player = PlayerActor;
		//if (OnShopOpen.IsBound())
		//{
			//OnShopOpen.Broadcast(CurrentShopItems, this);
		//}

		// E 눌렀을 때 HUD 중앙 패널 열기
		if (APawn* PlayerPawn = Cast<APawn>(PlayerActor))
		{
			if (APlayerController* PC = Cast<APlayerController>(PlayerPawn->GetController()))
			{
				if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
				{
					HUD->ShowShadyStore(this, CurrentShopItems);
				}
			}
		}
	}
	else
	{
		// UI 상으로 비활성화 되는 창 띄우기
		UE_LOG(LogTemp,Log,TEXT("이미 거래한 상인입니다."));
	}
}

void AShadyGuyActor::DetermineRarity()
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

	//UE_LOG(LogTemp, Log, TEXT("ShadyGuy Rarity Decided: %d"), (int32)CurrentRarity);
}

void AShadyGuyActor::UpdateMeshTexture()
{
	if (!ShadyGuyMesh) return;

	// 머티리얼 인스턴스 생성 (인덱스 1번 슬롯 가정, 상황에 따라 0번일 수 있음)
	UMaterialInstanceDynamic* DMI = ShadyGuyMesh->CreateAndSetMaterialInstanceDynamic(1);
	if (!DMI) return;

	UTexture2D* SelectedTexture = nullptr;

	switch (CurrentRarity)
	{
	case EItemGrade::Rare:
		SelectedTexture = RareTexture;
		break;
	case EItemGrade::Epic:
		SelectedTexture = EpicTexture;
		break;
	case EItemGrade::Legendary:
		SelectedTexture = LegendaryTexture;
		break;
	case EItemGrade::Common:
	default:
		SelectedTexture = CommonTexture;
		break;
	}

	if (SelectedTexture)
	{
		// 머티리얼의 텍스처 파라미터 이름 ("DiffuseColorMap")
		DMI->SetTextureParameterValue(FName("DiffuseColorMap"), SelectedTexture);
	}
}

void AShadyGuyActor::GenerateShopItems()
{
	CurrentShopItems.Empty();

	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("ShadyGuy: ItemDataTable is NULL!"));
		return;
	}

	static const FString ContextString(TEXT("ShopItemGeneration"));
	TArray<FName> RowNames = ItemDataTable->GetRowNames();
	TArray<FName> CandidateItemIDs;

	// 현재 등급(CurrentRarity)과 일치하는 아이템만 필터링
	for (const FName& RowName : RowNames)
	{
		FItemData* ItemData = ItemDataTable->FindRow<FItemData>(RowName, ContextString);
		if (ItemData && ItemData->Grade == CurrentRarity)
		{
			CandidateItemIDs.Add(RowName);
		}
	}

	// 후보군이 없으면 경고
	if (CandidateItemIDs.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShadyGuy: No items found for Rarity %d"), (int32)CurrentRarity);
		return;
	}

	// 셔플 (Fisher-Yates Shuffle) - 순서를 랜덤하게 섞음
	int32 LastIndex = CandidateItemIDs.Num() - 1;
	for (int32 i = 0; i <= LastIndex; ++i)
	{
		int32 RandomIndex = FMath::RandRange(i, LastIndex);
		if (i != RandomIndex)
		{
			CandidateItemIDs.Swap(i, RandomIndex);
		}
	}

	// 최대 3개까지 추출하여 리스트에 담기
	int32 ItemsToSpawn = FMath::Min(3, CandidateItemIDs.Num());
	for (int32 i = 0; i < ItemsToSpawn; ++i)
	{
		FName SelectedID = CandidateItemIDs[i];
		FItemData* ItemData = ItemDataTable->FindRow<FItemData>(SelectedID, ContextString);

		if (ItemData)
		{
			FShopSlotInfo NewSlot;

			NewSlot.ItemID = SelectedID;
			NewSlot.Price = ItemData->Price;

			NewSlot.ItemName = ItemData->Name;
			NewSlot.ItemDescription = ItemData->Description;
			NewSlot.ItemGrade = ItemData->Grade;

			if (!ItemData->Icon.IsNull())
			{
				NewSlot.IconTexture = ItemData->Icon.LoadSynchronous();
			}
			else
			{
				NewSlot.IconTexture = nullptr;
			}

			CurrentShopItems.Add(NewSlot);
			UE_LOG(LogTemp, Log, TEXT("Slot : %d , ItemId = %s"),i,*(NewSlot.ItemID).ToString());
		}
	}

	UE_LOG(LogTemp, Log, TEXT("ShadyGuy: Generated %d items for shop."), CurrentShopItems.Num());
}
