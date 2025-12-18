// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/SanctuaryBase.h"

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

void ASanctuaryBase::ApplyEffect_Implementation(AActor* Player)
{
}

