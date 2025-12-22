// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/MicrowaveActor.h"

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

// Called when the game starts or when spawned
void AMicrowaveActor::BeginPlay()
{
	Super::BeginPlay();

	if (MicrowaveMesh)
	{
		// 1. 다이내믹 머티리얼 인스턴스(DMI) 생성
		UMaterialInstanceDynamic* DMI = MicrowaveMesh->CreateAndSetMaterialInstanceDynamic(0);

		if (DMI)
		{
			// 1. 확률 계산 (총합 110)
			int32 RandomChance = FMath::RandRange(1, 110);

			// 2. 확률에 따라 등급(Rarity) 설정 (멤버 변수에 저장)
			if (RandomChance <= 50)
			{
				CurrentRarity = EItemGrade::Common; // 50 (1~50)
			}
			else if (RandomChance <= 80)
			{
				CurrentRarity = EItemGrade::UnCommon; // 30 (51~80)
			}
			else if (RandomChance <= 100)
			{
				CurrentRarity = EItemGrade::Rare; // 20 (81~100)
			}
			else
			{
				CurrentRarity = EItemGrade::Legendary; // 10 (101~110)
			}

			// 3. 결정된 등급에 따라 색상 적용
			FLinearColor SelectedColor;

			switch (CurrentRarity)
			{
			case EItemGrade::UnCommon:
				SelectedColor = FLinearColor(0.0f, 0.2f, 1.0f); // 파랑
				break;

			case EItemGrade::Rare:
				SelectedColor = FLinearColor(1.0f, 0.0f, 0.6f);
				break;

			case EItemGrade::Legendary:
				SelectedColor = FLinearColor(1.0f, 0.9f, 0.1f); // 노랑
				break;

			case EItemGrade::Common:
			default:
				SelectedColor = FLinearColor(1.0f, 1.0f, 1.0f); // 기본(흰색)
				break;
			}

			// 머티리얼 파라미터 변경
			DMI->SetVectorParameterValue(FName("PaintColor"), SelectedColor);

			// (디버깅용) 로그 출력
			UE_LOG(LogTemp, Warning, TEXT("Item Rarity: %d"), (int32)CurrentRarity);
		}
	}
}

void AMicrowaveActor::Interact_Implementation(AActor* PlayerActor)
{
	
}

