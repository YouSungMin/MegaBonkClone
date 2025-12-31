// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/ChestActor.h"
#include "Framework/MainHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/InventoryOwner.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AChestActor::AChestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(ChestMesh);
	ChestMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	ChestMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent); // 루트(메시)에 부착
	TriggerBox->SetBoxExtent(FVector(50.f, 50.f, 50.f)); // 크기 설정
	TriggerBox->SetCollisionProfileName(TEXT("Trigger")); // 트리거 프리셋 사용


	InteractionWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidgetComp->SetupAttachment(RootComponent); // 루트에 붙이기

	// ★핵심 설정: Screen 모드로 하면 카메라를 항상 정면으로 바라보고 크기가 일정하게 유지됨 (빌보드)
	InteractionWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	// 위젯 크기 설정 (원하는 대로 조절)
	InteractionWidgetComp->SetDrawAtDesiredSize(true);

	// 위치를 아이템 머리 위로 살짝 올림 (Z축 +80)
	InteractionWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));

	// 처음엔 안 보이게 숨김
	InteractionWidgetComp->SetVisibility(false);
}

// Called when the game starts or when spawned
void AChestActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (bAutoOpen)
	{
		if (TriggerBox)
		{
			TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AChestActor::OnOverlapBegin);
		}
	}
}

void AChestActor::Interact_Implementation(AActor* PlayerActor)
{
	if(!PlayerActor) return;

	if (!PlayerActor->Implements<UInventoryOwner>()) return;

	FName SelectedItemID = GetRandomItemID();

	if (SelectedItemID.IsNone())
	{
		UE_LOG(LogTemp,Log,TEXT("상자 오픈 실패, 획득 가능한 아이템이 없음"));
		Destroy();
		return;
	}

	if (OpenCost > 0)
	{
		int32 FinalCost = IInventoryOwner::Execute_GetAdjustedCost(PlayerActor, OpenCost);

		bool bPaymentSuccess = IInventoryOwner::Execute_UseGold(PlayerActor, FinalCost);

		if (!bPaymentSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("돈이 부족합니다!"));
			return;
		}

		IInventoryOwner::Execute_NotifyChestOpened(PlayerActor);
	}

	UE_LOG(LogTemp, Log,TEXT("아이템 추가 %s"),*SelectedItemID.ToString());
	IInventoryOwner::Execute_ReceiveItem(PlayerActor, SelectedItemID, 1);

	FItemData* ItemData = ItemDataTable->FindRow<FItemData>(SelectedItemID, TEXT("ChestLoot"));

	//HUD에 상자OpenUI띄우기 요청
	if (ItemData)
	{
		// PlayerActor(캐릭터) -> Controller -> HUD 순서로 접근
		if (APawn* PlayerPawn = Cast<APawn>(PlayerActor))
		{
			if (APlayerController* PC = Cast<APlayerController>(PlayerPawn->GetController()))
			{
				// HUD로 캐스팅해서 함수 호출
				if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
				{
					HUD->ShowChestReward(*ItemData);
				}
			}
		}
	}

	//상자 삭제 
	Destroy();
}

void AChestActor::BeginFocus_Implementation()
{
	if (InteractionWidgetComp)
	{
		InteractionWidgetComp->SetVisibility(true);
	}
}

void AChestActor::EndFocus_Implementation()
{
	if (InteractionWidgetComp)
	{
		InteractionWidgetComp->SetVisibility(false);
	}
}

void AChestActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bAutoOpen) return;

	// 이미 열렸거나 대상이 없으면 리턴
	if (bIsOpened || !OtherActor) return;

	// 플레이어인지 확인 후 열기
	if (OtherActor->Implements<UInventoryOwner>())
	{
		Interact_Implementation(OtherActor);
	}
}

FName AChestActor::GetRandomItemID()
{
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp,Log,TEXT("아이템 데이터 테이블이 존재하지 않습니다."));
		return NAME_None;
	}

	TArray<FLootCandidate> Candidates;
	float TotalWeight = 0.0f;

	// 1. 데이터 테이블의 모든 행(Row)을 가져옴
	TArray<FName> RowNames = ItemDataTable->GetRowNames();

	for (const FName& RowName : RowNames)
	{
		// 등급 확인을 위해 데이터 조회
		FItemData* Data = ItemDataTable->FindRow<FItemData>(RowName, TEXT("LootCalc"));

		// 데이터가 존재하고, 이 상자에 설정된 등급(TierProbabilities)에 포함되는지 확인
		if (Data && TierProbabilities.Contains(Data->Grade))
		{
			float Weight = TierProbabilities[Data->Grade];

			// 가중치가 0보다 클 때만 후보로 등록
			if (Weight > 0.0f)
			{
				Candidates.Add({ RowName, Weight });
				TotalWeight += Weight;
			}
		}
	}

	// 2. 후보가 없으면 빈 이름 반환
	if (Candidates.Num() == 0 || TotalWeight <= 0.0f)
	{
		return NAME_None;
	}

	// 3. 가중치 랜덤 뽑기 (Weighted Random)
	float RandomValue = FMath::FRandRange(0.0f, TotalWeight);

	for (const FLootCandidate& Candidate : Candidates)
	{
		RandomValue -= Candidate.Weight;
		if (RandomValue <= 0.0f)
		{
			return Candidate.ItemID; // 당첨된 ID 반환
		}
	}
	
	return Candidates.Last().ItemID;
}

