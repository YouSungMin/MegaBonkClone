// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeaponBaseActor.h"
#include "Weapons/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"//임시

// Sets default values
AProjectileWeaponBaseActor::AProjectileWeaponBaseActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AProjectileWeaponBaseActor::LaunchProjectile()
{
	if (UWorld* world = GetWorld()) {

		AActor* owner = this->GetOwner();

		// TrailClass가 할당되었는지 체크 (안전장치)
		if (ProjectileClass)
		{
			// 충돌 때문에 스폰 실패하는 것을 방지 (선택 사항)
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = owner;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			GetWorld()->SpawnActor<AProjectileBase>(
				ProjectileClass,
				GetActorLocation(),
				GetActorRotation(),
				SpawnParams
			);
		}
		
	}
}

// Called when the game starts or when spawned
void AProjectileWeaponBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(
		ProjectileTimerHandle,
		this,
		&AProjectileWeaponBaseActor::LaunchProjectile,
		1.0f,
		true
	);
}
