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

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = owner;     
		SpawnParams.Instigator = Cast<APawn>(owner);

		if (ProjectileClass) {
			AProjectileBase* projectile = world->SpawnActor<AProjectileBase>(
				ProjectileClass,
				owner->GetActorTransform(),
				SpawnParams
			);
		}
		
	}
}

// Called when the game starts or when spawned
void AProjectileWeaponBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
	//GetWorldTimerManager().SetTimer(
	//	ProjectileTimerHandle,         // 사용할 핸들
	//	this,                      // 함수가 있는 객체 (나 자신)
	//	&AProjectileWeaponBaseActor::LaunchProjectile, // 호출할 함수 주소
	//	ProjectileTimerTime,                      // 시간 간격 (1.0초)
	//	true                       // 반복 여부 (true: 계속 반복, false: 1번만 실행)
	//);

}
