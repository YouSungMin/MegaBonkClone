// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/RechargeSanctuary.h"
#include "Components/SphereComponent.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "TimerManager.h"

ARechargeSanctuary::ARechargeSanctuary()
{
	ProximitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProximitySphere"));
	ProximitySphere->SetupAttachment(Root);
	ProximitySphere->SetSphereRadius(150.0f);
	ProximitySphere->SetCollisionProfileName(TEXT("Trigger"));
}

void ARechargeSanctuary::BeginPlay()
{
	ProximitySphere->OnComponentBeginOverlap.AddDynamic(this, &ARechargeSanctuary::OnProximityBeginOverlap);
	ProximitySphere->OnComponentEndOverlap.AddDynamic(this, &ARechargeSanctuary::OnProximityEndOverlap);

	if (FMath::FRand() <= GoldChance)
	{
		bIsGoldSanctuary = true;
		UE_LOG(LogTemp, Warning, TEXT("금색 성소 등장"));
	}
}

void ARechargeSanctuary::ApplyEffect_Implementation(AActor* Player)
{
}

void ARechargeSanctuary::OnProximityBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		return;
	}
	OverlappingPlayer = OtherActor;

	// 타이머 시작
	GetWorldTimerManager().SetTimer(ChargeTimerHandle, this, &ARechargeSanctuary::OnChargeComplete, ChargeTime, false);

	UE_LOG(LogTemp, Log, TEXT("충전 시작"), ChargeTime);

	// UI 띄우기
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
	if (!OverlappingPlayer) return;

	UE_LOG(LogTemp, Log, TEXT("보상 선택 UI 출력"));
}
