// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"
#include "Characters/Components/StatusComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

   
	OnActorBeginOverlap.AddDynamic(this, &AWeaponBase::OnBeginWeaponOverlap);
	OnActorEndOverlap.AddDynamic(this, &AWeaponBase::OnEndWeaponOverlap);
	
}

