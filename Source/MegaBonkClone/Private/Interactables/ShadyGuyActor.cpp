// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/ShadyGuyActor.h"

// Sets default values
AShadyGuyActor::AShadyGuyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ShadyGuyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	ShadyGuyMesh->SetupAttachment(Root);
	ShadyGuyMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	ShadyGuyMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

// Called when the game starts or when spawned
void AShadyGuyActor::BeginPlay()
{
	Super::BeginPlay();
	
}