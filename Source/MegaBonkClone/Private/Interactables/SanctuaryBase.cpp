// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/SanctuaryBase.h"
#include "Components/WidgetComponent.h"
// Sets default values
ASanctuaryBase::ASanctuaryBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SantuaryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SantuaryMesh->SetupAttachment(Root);
	SantuaryMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	SantuaryMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// 위젯 컴포넌트 생성 및 설정
	InteractionWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidgetComp->SetupAttachment(RootComponent); // 루트에 붙이기

	// Screen 모드로 하면 카메라를 항상 정면으로 바라보고 크기가 일정하게 유지됨 (빌보드)
	InteractionWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	// 위젯 크기 설정 (원하는 대로 조절)
	InteractionWidgetComp->SetDrawAtDesiredSize(true);

	// 위치를 아이템 머리 위로 살짝 올림 (Z축 +80)
	InteractionWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));

	// 처음엔 안 보이게 숨김
	InteractionWidgetComp->SetVisibility(false);
}

// Called when the game starts or when spawned
void ASanctuaryBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASanctuaryBase::Interact_Implementation(AActor* PlayerActor)
{
	UE_LOG(LogTemp,Log,TEXT("성소 상호작용"));
	if (!bIsUsed)
	{
		ApplyEffect(PlayerActor);
		UE_LOG(LogTemp,Log,TEXT("성소 효과 적용"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("이미 사용한 성소입니다"));
	}
}

void ASanctuaryBase::BeginFocus_Implementation()
{
	if (InteractionWidgetComp)
	{
		InteractionWidgetComp->SetVisibility(true);
	}
}

void ASanctuaryBase::EndFocus_Implementation()
{
	if (InteractionWidgetComp)
	{
		InteractionWidgetComp->SetVisibility(false);
	}
}

void ASanctuaryBase::ApplyEffect_Implementation(AActor* Player)
{
}

