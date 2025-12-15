// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/PotActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APotActor::APotActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PotMesh"));
	SetRootComponent(PotMesh);
	PotMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

// Called when the game starts or when spawned
void APotActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APotActor::Interact_Implementation(AActor* InstigatorActor)
{
	
	// 깨지는 이펙트 (파티클) 재생
	if (BreakEffect)
	{

	}

	// 깨지는 소리 재생
	if (BreakSound)
	{

	}
}

