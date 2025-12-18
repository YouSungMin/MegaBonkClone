// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/MagneticSanctuary.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"

// Sets default values
AMagneticSanctuary::AMagneticSanctuary()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMagneticSanctuary::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMagneticSanctuary::ApplyEffect_Implementation(AActor* Player)
{
	Super::ApplyEffect_Implementation(Player);

	if (auto* player = Cast<APlayerCharacter>(Player))
	{
		player->ActivateMagnetEffect();
		bIsUsed = true;
	}
}

