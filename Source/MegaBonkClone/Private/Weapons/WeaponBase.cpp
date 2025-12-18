// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"
#include "Characters/Components/StatusComponent.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Weapons/ProjectileWeaponActor.h"
#include "Weapons/BoomerangProjectileWeaponActor.h"
#include "Weapons/TrailWeaponActor.h"
#include "Weapons/BouncingProjectileWeaponActor.h"
#include "Data/WeaponDataStructs.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h" 

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

	OwnerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	OwnerStatusComp = OwnerCharacter->GetStatusComponent();
	LoadWeaponData();
}

AActor* AWeaponBase::FindNearestEnemy(float SearchRadius)
{
	if (!GetWorld() || !OwnerCharacter.IsValid()) return nullptr;

	FVector MyLoc = OwnerCharacter->GetActorLocation();

	// 1. 탐색할 오브젝트 타입 설정 (Pawn = 캐릭터/몬스터)
	// 몬스터가 'Pawn' 채널을 쓴다고 가정합니다.
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	// 만약 몬스터가 WorldDynamic이면 아래 줄 주석 해제
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	// 2. 탐색 모양 설정 (구체)
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(SearchRadius);

	// 3. 오버랩 검사 실행
	TArray<FOverlapResult> OverlapResults;
	bool bHit = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		MyLoc,
		FQuat::Identity,
		ObjectQueryParams,
		CollisionShape
	);

	// 4. 결과 중에서 가장 가까운 적 찾기
	AActor* NearestEnemy = nullptr;
	float MinDistSq = FLT_MAX; // 최소 거리(제곱)

	if (bHit)
	{
		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* Enemy = Result.GetActor();

			// (중요) 유효성 검사 + "Enemy" 태그 확인 + 나 자신 제외
			if (Enemy && Enemy->ActorHasTag("Enemy") && IsValidTarget(Enemy))
			{
				float DistSq = FVector::DistSquared(MyLoc, Enemy->GetActorLocation());
				if (DistSq < MinDistSq)
				{
					MinDistSq = DistSq;
					NearestEnemy = Enemy;
				}
			}
		}
	}

	// 디버그용: 범위 눈으로 확인하기 (필요할 때만 주석 해제)
	
	DrawDebugSphere(GetWorld(), MyLoc, SearchRadius, 32, FColor::Red, false, 0.1f);
	if(NearestEnemy)
	{
		DrawDebugLine(GetWorld(), MyLoc, NearestEnemy->GetActorLocation(), FColor::Green, false, 0.1f, 0, 2.0f);
	}
	

	return NearestEnemy;
}

void AWeaponBase::LoadWeaponData()
{
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

bool AWeaponBase::CheckIsCritical()
{
	float totalCriticalChance = CriticalChance;

	if (OwnerStatusComp.IsValid()) {
		totalCriticalChance += OwnerStatusComp.Get()->GetResultCriticalChance();
		//UE_LOG(LogTemp, Warning, TEXT("totalCriticalChance : %.1f"), totalCriticalChance);
	}

	if (FMath::RandRange(0.0f, 1.0f) <= totalCriticalChance) {
		return true;
	}

	return false;
}

void AWeaponBase::StartAttackTimer()
{
	
	if (!OwnerStatusComp.IsValid()) return;
	//UE_LOG(LogTemp, Warning, TEXT("StartAttackTimer"));
	float AttackSpeedRate = OwnerStatusComp.Get()->GetResultAttackSpeed();
	if (AttackSpeedRate <= 0.01f) AttackSpeedRate = 0.01f;

	// 새로 계산된 간격
	float NewInterval = 3.0f / AttackSpeedRate;

	// [핵심] 기존 간격과 거의 차이가 없다면(변화 없음), 타이머를 건드리지 않고 리턴!
	if (FMath::IsNearlyEqual(CurrentAttackInterval, NewInterval, 0.001f))
	{
		return;
	}

	// 변화가 있다면 갱신
	CurrentAttackInterval = NewInterval;

	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	GetWorldTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&AWeaponBase::InvokeAttack,
		CurrentAttackInterval,
		true
	);
	
	

}

void AWeaponBase::InitializeWeaponStatus(const FWeaponData& InWeaponData)
{
	//UE_LOG(LogTemp, Warning, TEXT("InitializeWeaponStatus"));
	WeaponDamage = InWeaponData.WeaponDamage;
	ProjectileCount = InWeaponData.ProjectileCount;
	ProjectileSpeed = InWeaponData.ProjectileSpeed;
	AttackSize = InWeaponData.ProjectileScale;
	ProjectileReflectCount = InWeaponData.ChainCount;     
	CriticalChance = InWeaponData.CriticalChance;
	CritDmgRate = InWeaponData.CriticalDamage; 
	KnockBack = InWeaponData.KnockBack;

	if (Implements<UWeapon>()) {
		IWeapon::Execute_GetDamageWeapon(this);
	}

	StartAttackTimer();

}


void AWeaponBase::GetDamageWeapon_Implementation()
{
	if (OwnerStatusComp.IsValid()) {
		WeaponFinalDamage = WeaponDamage * OwnerStatusComp->GetResultDamage();
		//UE_LOG(LogTemp, Warning, TEXT("무기데미지 : %.1f"), WeaponFinalDamage);

		WeaponFinalCriticalDamage = (1.0f + (0.1f*CritDmgRate))* OwnerStatusComp->GetResultCritDmgRate()* WeaponFinalDamage;
		//UE_LOG(LogTemp, Warning, TEXT("무기크리데미지 : %.1f"), WeaponFinalCriticalDamage);
	}
}

void AWeaponBase::AttackWeapon_Implementation()
{
	UpdateWeaponStats();
	//UE_LOG(LogTemp, Warning, TEXT("%s : AttackWeapon_Implementation : %.1f"),*this->GetName(), WeaponFinalDamage);
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
	//같은 클래스 종류인지 확인
	if (OtherActor->IsA(AWeaponBase::StaticClass()))
	{
		return false;
	}
	if(OtherActor->IsA(AProjectileWeaponActor::StaticClass())
		|| OtherActor->IsA(ABoomerangProjectileWeaponActor::StaticClass())
		|| OtherActor->IsA(ABouncingProjectileWeaponActor::StaticClass())
			|| OtherActor->IsA(ATrailWeaponActor::StaticClass()))
	{
		return false;
	}
	return true;
}



void AWeaponBase::UpdateWeaponStats()
{
	if (!OwnerStatusComp.IsValid()) return;
	
	
	// 1. 발사체 수 (단순 합산)
	float StatusProjCount = OwnerStatusComp->GetResultProjectileCount();
	FinalProjectileCount = ProjectileCount + StatusProjCount;

	// 2. 발사체 속도 (배율 적용)
	// StatusComponent의 ResultProjectileSpeed가 %단위(100.0 = 100%)라고 가정
	float StatusSpeedPct = OwnerStatusComp->GetResultProjectileSpeed();
	float SpeedMult = StatusSpeedPct;
	if (SpeedMult <= 0.0f) SpeedMult = 1.0f; // 0이면 1배로 보정

	FinalProjectileSpeed = ProjectileSpeed * SpeedMult;

	// 3. 무기 크기 (배율 적용)
	float StatusSizePct = OwnerStatusComp->GetResultAttackSize();
	float SizeMult = StatusSizePct;
	if (SizeMult <= 0.0f) SizeMult = 1.0f;

	FinalAttackSize = AttackSize * SizeMult;


	// 4. 반사 횟수 (단순 합산)
	float StatusReflect = OwnerStatusComp->GetResultProjectileReflectCount();
	FinalReflectCount = ProjectileReflectCount + StatusReflect;
	UE_LOG(LogTemp, Warning, TEXT("FinalReflectCount : %.1f"), FinalReflectCount);

	// 5. 지속 시간 (배율 적용) - 필요 시
	float StatusDurPct = OwnerStatusComp->GetResultAttackDuration();
	float DurMult = StatusDurPct;
	if (DurMult <= 0.0f) DurMult = 1.0f;

	// WeaponData에 Duration 변수가 없다면 임시로 5.0f 사용
	float BaseDuration = 1.0f;
	FinalDuration = BaseDuration * DurMult;

	if (Implements<UWeapon>()) {
		IWeapon::Execute_GetDamageWeapon(this);
	}

	StartAttackTimer();
	
}