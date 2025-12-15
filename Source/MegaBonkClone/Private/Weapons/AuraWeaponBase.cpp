// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AuraWeaponBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AAuraWeaponBase::AAuraWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);
	BaseMesh->SetCollisionProfileName("NoCollision");
	RingMesh = CreateDefaultSubobject<UStaticMeshComponent>("RingMesh");
	RingMesh->SetupAttachment(BaseMesh);
	RingMesh->SetCollisionProfileName("NoCollision");
	Collision = CreateDefaultSubobject<UCapsuleComponent>("Collision");
	Collision->SetupAttachment(BaseMesh);
	
}



void AAuraWeaponBase::AttackWeapon_Implementation()
{
	Super::AttackWeapon_Implementation();
}

void AAuraWeaponBase::GetDamageWeapon_Implementation()
{
	Super::GetDamageWeapon_Implementation();
}

void AAuraWeaponBase::StartAttackTimer()
{
	Super::StartAttackTimer();
}


// Called when the game starts or when spawned
void AAuraWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}


