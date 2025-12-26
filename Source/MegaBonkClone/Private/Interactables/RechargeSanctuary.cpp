// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/RechargeSanctuary.h"
#include "Components/SphereComponent.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "TimerManager.h"

#include "Framework/MainHUD.h"
#include "GameFramework/PlayerController.h"
ARechargeSanctuary::ARechargeSanctuary()
{
	ProximitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProximitySphere"));
	ProximitySphere->SetupAttachment(Root);
	ProximitySphere->SetSphereRadius(150.0f);
	ProximitySphere->SetCollisionProfileName(TEXT("Trigger"));
}

void ARechargeSanctuary::BeginPlay()
{
	Super::BeginPlay();

	ProximitySphere->OnComponentBeginOverlap.AddDynamic(this, &ARechargeSanctuary::OnProximityBeginOverlap);
	ProximitySphere->OnComponentEndOverlap.AddDynamic(this, &ARechargeSanctuary::OnProximityEndOverlap);

	if (FMath::FRand() <= GoldChance)
	{
		bIsGoldSanctuary = true;
		UE_LOG(LogTemp, Warning, TEXT("금색 성소 등장"));
	}

	if (RarityWeights.Num() == 0)
	{
		RarityWeights.Add(EItemGrade::Common, 60.0f);
		RarityWeights.Add(EItemGrade::UnCommon, 30.0f);
		RarityWeights.Add(EItemGrade::Rare, 9.0f);
		RarityWeights.Add(EItemGrade::Legendary, 1.0f);
	}
}

void ARechargeSanctuary::ApplyEffect_Implementation(AActor* Player)
{
}

void ARechargeSanctuary::Interact_Implementation(AActor* PlayerActor)
{
	if (bIsUsed) return;
	if (!OverlappingPlayer.IsValid() || OverlappingPlayer.Get() != PlayerActor) return;

	GetWorldTimerManager().SetTimer(ChargeTimerHandle, this, &ARechargeSanctuary::OnChargeComplete, ChargeTime, false);

}

void ARechargeSanctuary::ApplySelectedReward(const FSanctuaryRewardInfo& RewardInfo)
{
	if (!OverlappingPlayer.IsValid())

	{
		UE_LOG(LogTemp, Error, TEXT("보상을 적용하려 했으나 플레이어가 더 이상 유효하지 않습니다."));

		return;
	}

	AActor* PlayerActor = OverlappingPlayer.Get();

	bIsUsed = true;
	UE_LOG(LogTemp, Log, TEXT("보상 생성: %d (%d) -> Value: %.1f"), RewardInfo.StatType, RewardInfo.Rarity, RewardInfo.Value);
}

void ARechargeSanctuary::OnProximityBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsUsed)
	{
			UE_LOG(LogTemp, Log, TEXT("충전 시작"), ChargeTime);
			if (!OtherActor->IsA(APlayerCharacter::StaticClass()))
			{
				return;
			}
			OverlappingPlayer = OtherActor;

			// 타이머 시작 //자동시작 막기
			//GetWorldTimerManager().SetTimer(ChargeTimerHandle, this, &ARechargeSanctuary::OnChargeComplete, ChargeTime, false);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("이미 사용한 성소입니다"));
	}
}

void ARechargeSanctuary::OnProximityEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	if (OtherActor == OverlappingPlayer)
	{
		// 타이머 취소
		GetWorldTimerManager().ClearTimer(ChargeTimerHandle);
		OverlappingPlayer = nullptr;

		UE_LOG(LogTemp, Log, TEXT("범위를 벗어나 충전이 취소되었습니다."));

		// TODO: UI 끄기
	}
}


void ARechargeSanctuary::OnChargeComplete()
{
	if (!OverlappingPlayer.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("충전 완료되었으나 플레이어가 유효하지 않습니다. (사망/로그아웃)"));
		// 필요하다면 타이머 핸들 초기화나 상태 리셋

		return;
	}

	if (AvailableStats.Num() == 0) return;

	TArray<FSanctuaryRewardInfo> FinalRewards;
	TArray<EItemStatType> TempPool = AvailableStats; // 중복 방지용

	int32 PickCount = FMath::Min(3, TempPool.Num());

	for (int32 i = 0; i < PickCount; i++)
	{
		FSanctuaryRewardInfo Reward;

		// 1. 스탯 랜덤 (균등 확률)
		int32 RandIdx = FMath::RandRange(0, TempPool.Num() - 1);
		Reward.StatType = TempPool[RandIdx];
		TempPool.RemoveAtSwap(RandIdx);

		// 2. 등급 랜덤 (가중치 확률)
		Reward.Rarity = PickRandomRarity();

		// 3. [핵심] 수치 계산: 기본값 * 배율
		float BaseVal = 0.0f;
		if (StatBaseValues.Contains(Reward.StatType))
		{
			BaseVal = StatBaseValues[Reward.StatType];
		}
		else
		{
			// 설정 안 된 스탯은 기본 10.0으로 가정 (안전장치)
			BaseVal = 10.0f;
		}

		float Multiplier = 1.0f;
		if (RarityMultipliers.Contains(Reward.Rarity))
		{
			Multiplier = RarityMultipliers[Reward.Rarity];
		}

		// 최종값 계산 (소수점 정리 필요 시 FMath::RoundToFloat 등 사용)
		Reward.Value = BaseVal * Multiplier;

		FinalRewards.Add(Reward);

		// 로그 출력 (디버깅용)
		
		FString StatName = UEnum::GetDisplayValueAsText(Reward.StatType).ToString();
		FString GradeName = UEnum::GetDisplayValueAsText(Reward.Rarity).ToString();
		UE_LOG(LogTemp, Log, TEXT("보상 생성: %s (%s) -> Value: %.1f"), *StatName, *GradeName, Reward.Value);
		
	}

	//1. HUD로 UI열기
	if (APawn* PlayerPawn = Cast<APawn>(OverlappingPlayer.Get()))
	{
		if (APlayerController* PC = Cast<APlayerController>(PlayerPawn->GetController()))
		{
			if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
			{
				HUD->ShowRechargeSanctuary(this);
			}
		}
	}

		// UI 호출
	if (OnRewardsGenerated.IsBound())
	{
			OnRewardsGenerated.Broadcast(FinalRewards, this);
	}

	//bIsCharging = false;
}

EItemGrade ARechargeSanctuary::PickRandomRarity()
{
	float TotalWeight = 0.0f;
	for (const auto& Entry : RarityWeights)
	{
		TotalWeight += Entry.Value;
	}

	float RandomPoint = FMath::FRandRange(0.0f, TotalWeight);
	float CurrentWeight = 0.0f;

	for (const auto& Entry : RarityWeights)
	{
		CurrentWeight += Entry.Value;
		if (RandomPoint <= CurrentWeight)
		{
			return Entry.Key;
		}
	}

	return EItemGrade::Common;
}
