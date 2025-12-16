// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/ChestActor.h"

// Sets default values
AChestActor::AChestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

