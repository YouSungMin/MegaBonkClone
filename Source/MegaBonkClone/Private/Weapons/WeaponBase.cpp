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

void AWeaponBase::OnBeginWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

}

bool AWeaponBase::IsValidTarget(AActor* OtherActor)
{
	// 1. 유효성 검사 및 자기 자신과의 충돌 방지
	if (OtherActor == nullptr || OtherActor == this)
	{
		return false;
	}
	// 2. 같은 클래스 종류(ATrailWeaponActor 및 이를 상속받은 모든 자식)인지 확인
	if (OtherActor->IsA(AWeaponBase::StaticClass()))
	{
		return false;
	}
	return true;
}

void AWeaponBase::OnEndWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

}