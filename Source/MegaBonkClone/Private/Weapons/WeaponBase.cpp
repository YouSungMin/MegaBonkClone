// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"
#include "Characters/Components/StatusComponent.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Data/WeaponDataStructs.h"
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

	OwnerStatusComp = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->GetStatusComponent();
	// 1. 데이터 테이블 핸들이 유효한지 확인
	if (!WeaponTableRow.IsNull())
	{
		// 2. 핸들을 통해 실제 데이터(Row) 가져오기
		// GetRow<구조체타입>(ContextString)
		FWeaponData* RowData = WeaponTableRow.GetRow<FWeaponData>(TEXT("Weapon Data Context"));

		// 3. 데이터가 잘 가져와졌다면 초기화 함수 실행
		if (RowData)
		{
			// 기존에 만들어두신 InitializeWeapon 함수 재활용
			// 포인터(*RowData)를 참조(&)로 넘김
			InitializeWeaponStatus(*RowData);

			// 로그로 확인 (선택 사항)
			UE_LOG(LogTemp, Log, TEXT("Weapon Loaded: %s, Damage: %f"), *RowData->Name.ToString(), RowData->WeaponDamage);
		}
	}
}

void AWeaponBase::InvokeAttack()
{
	//UE_LOG(LogTemp, Warning, TEXT("InvokeAttack"));
	if (Implements<UWeapon>()) {
		IWeapon::Execute_AttackWeapon(this);
	}
}

void AWeaponBase::StartAttackTimer()
{
	

	if (!OwnerStatusComp.IsValid())return;
	UE_LOG(LogTemp, Warning, TEXT("StartAttackTimer"));
	if (OwnerStatusComp.IsValid()) {
		float attackSpeed = OwnerStatusComp.Get()->GetAttackSpeed();
		UE_LOG(LogTemp, Log, TEXT("attackSpeed: %f"), attackSpeed);
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
	
	

}

void AWeaponBase::InitializeWeaponStatus(const FWeaponData& InWeaponData)
{
	//UE_LOG(LogTemp, Warning, TEXT("InitializeWeaponStatus"));
	WeaponDamage = InWeaponData.WeaponDamage;
	ProjectileCount = InWeaponData.ProjectileCount;
	ProjectileSpeed = InWeaponData.ProjectileSpeed;
	ProjectileAttackSize = InWeaponData.ProjectileScale; 
	ProjectileReflectCount = InWeaponData.ChainCount;     
	CriticalChance = InWeaponData.CriticalChance;
	CritDmgRate = InWeaponData.CriticalDamage; 
	KnockBack = InWeaponData.KnockBack;

	StartAttackTimer();
	//if (!OwnerStatusComp.IsValid())
	//{
	//	// GetOwner()로 주인을 가져옵니다. (SpawnParams.Owner 설정 필수)
	//	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	//	{
	//		OwnerStatusComp = Player->GetStatusComponent();
	//	}
	//}	
}


void AWeaponBase::GetDamageWeapon_Implementation()
{
	if (OwnerStatusComp.IsValid()) {
		WeaponFinalDamage = OwnerStatusComp->GetStatusDamage();
		UE_LOG(LogTemp, Warning, TEXT("데미지 : %.1f"),GetFinalDamage());
	}
}

void AWeaponBase::AttackWeapon_Implementation()
{
	if (Implements<UWeapon>()) {
		IWeapon::Execute_GetDamageWeapon(this);
		UE_LOG(LogTemp, Warning, TEXT("AttackWeapon_Implementation : %.1f"), WeaponFinalDamage);
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
