// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/MoaiSanctuary.h"
#include "Data/ItemDataStructs.h"
#include "Framework/MainHUD.h"

void AMoaiSanctuary::BeginPlay()
{
	// 확률 데이터가 비어있으면 기본값 세팅 (안전장치)
	if (RarityDropRates.Num() == 0)
	{
		RarityDropRates.Add(EItemGrade::Common, 60.0f);
		RarityDropRates.Add(EItemGrade::Rare, 30.0f);
		RarityDropRates.Add(EItemGrade::Epic, 9.0f);
		RarityDropRates.Add(EItemGrade::Legendary, 1.0f);
	}

	// 게임 시작 시 아이템 생성
	GenerateMoaiItems();
}

void AMoaiSanctuary::Interact_Implementation(AActor* PlayerActor)
{
    if (!bIsUsed)
    {
        Player = PlayerActor;

        if (CurrentMoaiItems.Num() == 0)
        {
            GenerateMoaiItems();
        }

        if (APawn* PlayerPawn = Cast<APawn>(PlayerActor))
        {
            if (APlayerController* PC = Cast<APlayerController>(PlayerPawn->GetController()))
            {
                if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
                {
                    //HUD->ShowMoaiSantuary(this, CurrentMoaiItems);
                }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("이미 사용된 성소입니다."));
    }
}

void AMoaiSanctuary::GenerateMoaiItems()
{
	CurrentMoaiItems.Empty();

	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("MoaiSanctuary: ItemDataTable is NULL!"));
		return;
	}

	static const FString ContextString(TEXT("MoaiItemGeneration"));
	TArray<FName> AllRowNames = ItemDataTable->GetRowNames();

	TSet<FName> SelectedIDs;

	int32 MaxAttempts = 50; // 무한 루프 방지용 안전장치
	int32 Attempts = 0;

    while (CurrentMoaiItems.Num() < 3 && Attempts < MaxAttempts)
    {
        Attempts++;

        // 이번 시행의 등급 결정
        EItemGrade SelectedRarity = GetRandomRarity();

        // 해당 등급이면서 아직 뽑지 않은 아이템만 후보군으로 등록
        TArray<FName> CandidateItemIDs;
        for (const FName& RowName : AllRowNames)
        {
            // 이미 뽑힌 아이템이면 건너뜀 (중복 제거 핵심)
            if (SelectedIDs.Contains(RowName)) continue;

            FItemData* ItemData = ItemDataTable->FindRow<FItemData>(RowName, ContextString);
            if (ItemData && ItemData->Grade == SelectedRarity)
            {
                CandidateItemIDs.Add(RowName);
            }
        }

        // 후보가 없다면 다시 뽑기
        if (CandidateItemIDs.Num() == 0)
        {
            // 루프 처음으로 돌아가서 등급부터 다시 뽑음
            continue;
        }

        // 후보 중 하나 랜덤 선택
        int32 RandIdx = FMath::RandRange(0, CandidateItemIDs.Num() - 1);
        FName PickedID = CandidateItemIDs[RandIdx];
        FItemData* PickedData = ItemDataTable->FindRow<FItemData>(PickedID, ContextString);

        if (PickedData)
        {
            FMoaiSlotInfo NewSlot;
            NewSlot.ItemID = PickedID;
            NewSlot.ItemName = PickedData->Name;
            NewSlot.ItemDescription = PickedData->Description;
            NewSlot.ItemGrade = PickedData->Grade;

            if (!PickedData->Icon.IsNull())
            {
                NewSlot.IconTexture = PickedData->Icon.LoadSynchronous();
            }

            CurrentMoaiItems.Add(NewSlot);

            SelectedIDs.Add(PickedID);

            UE_LOG(LogTemp, Log, TEXT("Moai Slot %d : [%s] %s (시도 횟수: %d)"),
                CurrentMoaiItems.Num(),
                *UEnum::GetValueAsString(SelectedRarity),
                *NewSlot.ItemID.ToString(),
                Attempts);
        }
    }

    if (CurrentMoaiItems.Num() < 3)
    {
        UE_LOG(LogTemp, Warning, TEXT("아이템 데이터 부족으로 3개를 채우지 못했습니다."));
    }
}

EItemGrade AMoaiSanctuary::GetRandomRarity()
{
	float TotalWeight = 0.0f;
	for (const auto& Entry : RarityDropRates)
	{
		TotalWeight += Entry.Value;
	}

	// 랜덤 값 뽑기 (0 ~ TotalWeight)
	float RandomValue = FMath::RandRange(0.0f, TotalWeight);
	float CurrentWeightSum = 0.0f;

	// 가중치 누적하며 범위 확인
	for (const auto& Entry : RarityDropRates)
	{
		CurrentWeightSum += Entry.Value;
		if (RandomValue <= CurrentWeightSum)
		{
			return Entry.Key;
		}
	}

	return EItemGrade::Common; // 기본값
}
