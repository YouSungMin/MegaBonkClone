// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AWeaponBase::AttackWeapon_Implementation()
{
}

void AWeaponBase::GetDamageWeapon_Implementation()
{
}

void AWeaponBase::StartAttackTimer()
{
}

float AWeaponBase::GetFinalDamage() const
{
	return 0.0f;
}

float AWeaponBase::GetFinalCooldown() const
{
	return 0.0f;
}

float AWeaponBase::GetFinalArea() const
{
	return 0.0f;
}

//void AWeaponBase::StartAttackTimer()
//{
//    GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
//
//    float FinalCooldown = GetFinalCooldown();
//
//    // 쿨타임마다 Fire 함수 호출 (Loop = true)
//    if (FinalCooldown > 0.0f)
//    {
//        GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AWeaponBase::AttackWeapon_Implementation, FinalCooldown, true);
//
//        // 시작하자마자 한 번 쏘기
//        AttackWeapon_Implementation();
//    }
//}
//
// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

   

	
}

