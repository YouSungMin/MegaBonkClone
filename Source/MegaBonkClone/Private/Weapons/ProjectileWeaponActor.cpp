// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeaponActor.h"
#include "Components/CapsuleComponent.h"
#include "Framework/ObjectPoolSubsystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MegaBonkClone/MegaBonkClone.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileWeaponActor::AProjectileWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//콜리전 설정
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
	SetRootComponent(CollisionComp);
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // 겹침 감지
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileWeaponActor::OnProjectileHit);

	//메쉬 설정
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	//투사체 컴포넌트 셋팅
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->ProjectileGravityScale = 0.0f; // 중력 0 (직사)
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->MaxSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = false;
	MovementComp->bShouldBounce = false;

	RotatingComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingComp"));
	RotatingComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
}

void AProjectileWeaponActor::InitializeProjectile(float InDamage, float InSpeed, float InRange, float InKnockback, bool bIsPenetrate)
{
	float BaseSpeedMultiplier = 5000.0f;

	Damage = InDamage;
	Knockback = InKnockback;
	bPenetrate = bIsPenetrate;

	// 속도 적용
	if (MovementComp)
	{
		MovementComp->InitialSpeed = InSpeed * BaseSpeedMultiplier;
		MovementComp->MaxSpeed = InSpeed * BaseSpeedMultiplier;

		//UE_LOG(LogTemp, Warning, TEXT("InitialSpeed : %.1f"),MovementComp->InitialSpeed);
	}

	// 사거리(지속시간) 적용: 속도 * 시간 = 거리 이므로, 시간 = 거리 / 속도
	// 만약 InRange가 '지속시간(초)' 단위라면 그냥 SetLifeSpan(InRange) 하면 됩니다.
	// 여기서는 InRange를 '지속시간(Duration)'으로 가정하겠습니다.
	if (InRange > 0.0f)
	{
		SetLifeSpan(InRange);
	}
}

void AProjectileWeaponActor::OnPoolActivate_Implementation()
{
	// 1. 다시 보이게 하고 틱 켜기
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	// 2. 무브먼트 컴포넌트 리셋 (멈춰있던거 다시 날리기)
	if (MovementComp)
	{
		MovementComp->SetUpdatedComponent(RootComponent);
		MovementComp->Velocity = GetActorForwardVector() * MovementComp->InitialSpeed;
		MovementComp->Activate();
	}
}

void AProjectileWeaponActor::OnPoolDeactivate_Implementation()
{
	// 1. 숨기고 끄기
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	// 2. 무브먼트 정지
	if (MovementComp)
	{
		MovementComp->StopMovementImmediately();
		MovementComp->Deactivate();
	}

	// 3. 타이머 클리어 (중요!)
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void AProjectileWeaponActor::LifeSpanExpired()
{
	if (UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
	{
		Pool->ReturnToPool(this);
	}
	else
	{
		UE_LOG(LogWeapon, Warning, TEXT("서브시스템 : Null"));
		Super::LifeSpanExpired(); // 서브시스템 없으면 그냥 파괴
	}
}

// Called when the game starts or when spawned
void AProjectileWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
	//SetLifeSpan(10.0f);
}

void AProjectileWeaponActor::OnProjectileHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Enemy 태그를 가져야 hit함
	if (OtherActor && OtherActor->ActorHasTag("Enemy"))
	{
		// 데미지 전달
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

		// 관통 옵션이 꺼져있으면, 한 놈 맞추고 사라짐
		if (!bPenetrate)
		{
			// Destroy() -> ReturnToPool()
			if (UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
			{
				Pool->ReturnToPool(this);
			}
			else
			{
				Destroy();
			}
		}
	}
}


