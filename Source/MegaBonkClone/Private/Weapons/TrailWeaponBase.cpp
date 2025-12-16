// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/TrailWeaponBase.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Characters/Components/StatusComponent.h"
#include "Weapons/TrailWeaponActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATrailWeaponBase::ATrailWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATrailWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}



void ATrailWeaponBase::AttackWeapon_Implementation()
{
	if (!TrailClass) return;

	FVector spawnLoction = OwnerCharacter->GetActorLocation();
	spawnLoction.Z = 0.0f;

	FRotator SpawnRot = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

	// 3. 장판 생성
	FActorSpawnParameters Params;
	Params.Owner = this; // 무기가 주인

	ATrailWeaponActor* NewTrail = GetWorld()->SpawnActor<ATrailWeaponActor>(TrailClass, spawnLoction, SpawnRot, Params);

	if (NewTrail)
	{
		// 4. 데이터 주입
		float FinalDamage = GetFinalDamage();
		float Size = ProjectileAttackSize > 0.0f ? ProjectileAttackSize : 1.0f;
		float Duration = OwnerStatusComp.Get()->GetAttackDuration()+1.0f; 
		float attackSpeed = OwnerStatusComp.Get()->GetAttackSpeed();
		// (데미지, 지속시간, 크기, 공격속도)
		NewTrail->InitializeTrail(FinalDamage, Duration, Size, attackSpeed);
	}
}

