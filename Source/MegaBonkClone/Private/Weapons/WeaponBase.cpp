// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"
#include "Characters/Components/StatusComponent.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

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

	//OnActorBeginOverlap.AddDynamic(this, &AWeaponBase::OnBeginWeaponOverlap);
	//OnActorEndOverlap.AddDynamic(this, &AWeaponBase::OnEndWeaponOverlap);

	OwnerStatusComp = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->GetStatusComponent();
	
}

void AWeaponBase::InvokeAttack()
{
	if (Implements<UWeapon>()) {
		IWeapon::Execute_AttackWeapon(this);
	}
}

void AWeaponBase::StartAttackTimer()
{
	if (!OwnerStatusComp.IsValid()) return;

	float attackSpeed = OwnerStatusComp->GetAttackSpeed();

	//기존 타이머가 돌고 있다면 초기화 (스탯 변경 시 재설정 위함)
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);

	//타이머 설정
	//InvokeAttack 함수를 attackSpeed마다 반복(loop) 실행
	GetWorldTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&AWeaponBase::InvokeAttack,
		attackSpeed,
		true
	);

}

void AWeaponBase::InitializeWeaponStatus()
{
}

void AWeaponBase::GetDamageWeapon_Implementation()
{
	if (OwnerStatusComp.IsValid()) {
		WeaponFinalDamage = OwnerStatusComp->GetStatusDamage();
		UE_LOG(LogTemp, Warning, TEXT("데미지 : %.1f"),WeaponFinalDamage);
	}
}


bool AWeaponBase::IsValidTarget(AActor* OtherActor)
{
	//플레이어 캐릭터인지 확인
	if (UGameplayStatics::GetPlayerPawn(GetWorld(), 0) == OtherActor) {
		return false;
	}
	//유효성 검사 및 자기 자신과의 충돌 방지
	if (OtherActor == nullptr || OtherActor == this)
	{
		return false;
	}
	//같은 클래스 종류(ATrailWeaponActor 및 이를 상속받은 모든 자식)인지 확인
	if (OtherActor->IsA(AWeaponBase::StaticClass()))
	{
		return false;
	}
	return true;
}



float AWeaponBase::GetFinalDamage() const
{
	return WeaponDamage* WeaponFinalDamage;
}
