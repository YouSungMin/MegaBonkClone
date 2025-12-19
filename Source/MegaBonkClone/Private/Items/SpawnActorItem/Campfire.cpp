// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SpawnActorItem/Campfire.h"
#include "Components/SphereComponent.h"

// Sets default values
ACampfire::ACampfire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CampFireMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	CampFireMesh->SetupAttachment(Root);
	CampFireMesh->SetCollisionProfileName(TEXT("NoCollision"));
	CampFireMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	HealthSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HealthSphere"));
	HealthSphere->SetupAttachment(Root);
	HealthSphere->SetSphereRadius(150.0f);
	HealthSphere->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void ACampfire::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACampfire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

