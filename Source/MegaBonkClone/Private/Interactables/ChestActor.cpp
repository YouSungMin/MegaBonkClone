// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/ChestActor.h"
#include "Interfaces/InventoryOwner.h"

// Sets default values
AChestActor::AChestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(ChestMesh);
	ChestMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	ChestMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

// Called when the game starts or when spawned
void AChestActor::BeginPlay()
{
	Super::BeginPlay();
	
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



	// 아이템 획득 이펙트 추가
	//Destroy();
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

