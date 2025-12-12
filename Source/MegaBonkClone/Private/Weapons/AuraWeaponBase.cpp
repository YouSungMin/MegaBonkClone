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
	
}

void AAuraWeaponBase::GetDamageWeapon_Implementation()
{
}

void AAuraWeaponBase::StartAttackTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

	float FinalCooldown = GetFinalCooldown();

	// 쿨타임마다 Fire 함수 호출 (Loop = true)
	if (FinalCooldown > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			AttackTimerHandle, this,
			&AWeaponBase::AttackWeapon_Implementation,
			FinalCooldown,
			true);

		// 시작하자마자 한 번 쏘기
		AttackWeapon_Implementation();
	}
}

void AAuraWeaponBase::OnBeginWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnBeginWeaponOverlap(OverlappedActor, OtherActor);

	if (IsValidTarget(OtherActor)) {
		StartAttackTimer();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	}
	
}

void AAuraWeaponBase::OnEndWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnEndWeaponOverlap(OverlappedActor, OtherActor);

	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}

// Called when the game starts or when spawned
void AAuraWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}


