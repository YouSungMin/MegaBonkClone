// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeaponActor.h"

// Sets default values
AProjectileWeaponActor::AProjectileWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectileWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

