// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/TrailWeaponActor.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATrailWeaponActor::ATrailWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Root);

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	Effect->SetupAttachment(Root);

}



// Called when the game starts or when spawned
void ATrailWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(1.0f);

}

