// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"//임시

// Sets default values
AProjectileWeaponBase::AProjectileWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);
	BaseMesh->SetCollisionProfileName("NoCollision");

	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	Collision->SetupAttachment(BaseMesh);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileComponent->bIsHomingProjectile = true;
	ProjectileComponent->InitialSpeed = 1500.0f;
	ProjectileComponent->MaxSpeed = 1500.0f;
	ProjectileComponent->bRotationFollowsVelocity = false; // 이동 방향으로 회전하지 않음 (바나나는 따로 돌거니까)
	ProjectileComponent->bShouldBounce = false; // 튕기기 끔
	ProjectileComponent->ProjectileGravityScale = 0.0f; // 중력 무시 (직사)


	RotatingComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingComponent"));
	RotatingComponent->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
}

// Called when the game starts or when spawned
void AProjectileWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	// 지정된 시간(ReturnDelay) 후에 StartReturn 함수 실행
	OwnerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	GetWorldTimerManager().SetTimer(ReturnTimerHandle, this, &AProjectileWeaponBase::StartReturn, ReturnDelay, false);
	SetHomingTarget(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void AProjectileWeaponBase::OnBeginWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnBeginWeaponOverlap(OverlappedActor, OtherActor);
	//캐릭터 일때 처리
	if (bIsReturning && OtherActor == OwnerPawn)
	{
		// 여기서 무기를 제거하거나, 쿨타임 초기화 등의 로직 수행
		//무기제거 나중에 오브젝트풀 사용예정

		Destroy();
		return;
	}

	//적일때 처리
	if (IsValidTarget(OtherActor)) {
		
	}
}

void AProjectileWeaponBase::OnEndWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnEndWeaponOverlap(OverlappedActor, OtherActor);
	if (IsValidTarget(OtherActor)) {

	}
}

void AProjectileWeaponBase::StartReturn()
{
	// 타이머가 발동되면 "유도탄(Homing)" 모드로 전환
	if (OwnerPawn)
	{
		//이제부터는 돌아오는 상태임을 명시
		bIsReturning = true;

		// 1. 유도탄 타겟 설정 (플레이어의 RootComponent)
		ProjectileComponent->HomingTargetComponent = OwnerPawn->GetRootComponent();

		// 2. 유도탄 모드 활성화
		ProjectileComponent->bIsHomingProjectile = true;

		// 3. 유도 가속도 설정 (높을수록 빨리 꺾어서 돌아옴)
		ProjectileComponent->HomingAccelerationMagnitude = HomingAccel;

		// (선택사항) 돌아올 때 속도를 조금 더 높이고 싶다면
		ProjectileComponent->MaxSpeed = 2000.0f;
	}
	else
	{
		//예외처리
		UE_LOG(LogTemp, Warning, TEXT("OwnerPawn없음"));
		Destroy();
	}
}

void AProjectileWeaponBase::SetHomingTarget(AActor* Target)
{
	//UE_LOG(LogTemp, Warning, TEXT("SetHomingTarget"));
	ProjectileComponent->HomingTargetComponent = Target->GetRootComponent();
}

