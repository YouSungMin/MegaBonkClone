// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeaponBaseActor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileWeaponBaseActor::AProjectileWeaponBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);
	BaseMesh->SetCollisionProfileName("NoCollision");

	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	Collision->SetupAttachment(BaseMesh);

	ProjectileComponent= CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
}

// Called when the game starts or when spawned
void AProjectileWeaponBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}