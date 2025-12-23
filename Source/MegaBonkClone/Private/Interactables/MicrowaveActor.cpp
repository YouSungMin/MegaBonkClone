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
			// TMap에 데이터가 없으면 기본값으로 방어 코드 실행
			if (RarityDropRates.Num() == 0)
			{
				RarityDropRates.Add(EItemGrade::Common, 60.0f);
				RarityDropRates.Add(EItemGrade::UnCommon, 30.0f);
				RarityDropRates.Add(EItemGrade::Rare, 10.0f);
				RarityDropRates.Add(EItemGrade::Legendary, 5.0f);
			}

			if (DMI)
			{
				// 전체 가중치 합계 계산
				float TotalWeight = 0.0f;
				for (const auto& Entry : RarityDropRates)
				{
					TotalWeight += Entry.Value;
				}

				// 0 ~ TotalWeight 사이의 랜덤 값 뽑기
				float RandomValue = FMath::RandRange(0.0f, TotalWeight);

				// 누적 가중치로 등급 결정
				float CurrentWeightSum = 0.0f;

				// 기본값 설정
				CurrentRarity = EItemGrade::Common;

				for (const auto& Entry : RarityDropRates)
				{
					CurrentWeightSum += Entry.Value;

					// 랜덤 값이 현재 누적 범위 안에 들어오면 당첨
					if (RandomValue <= CurrentWeightSum)
					{
						CurrentRarity = Entry.Key;
						break; // 결정되었으므로 반복문 탈출
					}
				}

				// 결정된 등급에 따라 텍스처 선택
				UTexture2D* SelectedTexture = nullptr;

				switch (CurrentRarity)
				{
				case EItemGrade::UnCommon:
					SelectedTexture = UncommonTexture;
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

				// 5. 텍스처가 유효하다면 머티리얼 파라미터 변경
				if (SelectedTexture != nullptr)
				{
					// 주의: 머티리얼 에디터의 텍스처 파라미터 이름과 정확히 일치해야 합니다.
					// 올려주신 파일 정보를 보면 이름이 'DiffuseColorMap'입니다.
					DMI->SetTextureParameterValue(FName("DiffuseColorMap"), SelectedTexture);
				}

				// (디버깅용) 로그 출력
				UE_LOG(LogTemp, Warning, TEXT("Item Rarity: %d"), (int32)CurrentRarity);
			}
		}
	}
}

void AMicrowaveActor::Interact_Implementation(AActor* PlayerActor)
{
	
}

