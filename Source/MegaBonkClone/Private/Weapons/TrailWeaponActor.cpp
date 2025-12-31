// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/TrailWeaponActor.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/ObjectPoolSubsystem.h"

// Sets default values
ATrailWeaponActor::ATrailWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	OverlapComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("OverlapComp"));
	OverlapComp->SetupAttachment(Root);
	OverlapComp->SetCapsuleHalfHeight(100.0f);
	OverlapComp->SetCapsuleRadius(100.0f);
	OverlapComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	SpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComp"));
	SpriteComp->SetupAttachment(Root);
	SpriteComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	SpriteComp->SetSpriteColor(FLinearColor(1.0f, 0.3f, 0.05f, 0.85f));
	SpriteComp->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	SpriteComp->SetCollisionProfileName("NoCollision");

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SpriteComp);
	MeshComp->SetCollisionProfileName("NoCollision");
	MeshComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	MeshComp->SetRelativeScale3D(FVector(0.19f, 0.19f, 0.01f));

	ScaleTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ScaleTimeline"));

}

void ATrailWeaponActor::InitializeTrail(float InDamage, float InCritDmg, float InCritChance, float InDuration, float InScale, float AttackSpeed)
{
	WeaponFinalDamage = InDamage;
	WeaponFinalCriticalDamage = InCritDmg;
	WeaponFinalCriticalChance = InCritChance;

	//UE_LOG(LogTemp, Warning, TEXT("Damage : %.1f SetLifeSpan: %.1f"), Damage, InDuration);
	// 1. 크기 적용
	SetActorScale3D(FVector(InScale));

	// 2. 수명 적용 (지속시간 끝나면 알아서 사라짐)
	SetLifeSpan(InDuration);

	// 3. 데미지 주기 타이머 시작 (예: 0.5초마다 틱)
	GetWorldTimerManager().SetTimer(
		DamageTimerHandle,
		this,
		&ATrailWeaponActor::OnDamageTick,
		AttackSpeed * 0.5f,
		true,
		0.0f
	);
	
	InitialScale = FVector(InScale);
	SetActorScale3D(InitialScale);

	// [핵심] 전체 수명에서 '커브 길이(0.5초)'를 뺀 시간에 타이머를 겁니다.
	// 예: 5초 지속이면, 4.5초 뒤에 작아지기 시작.
	float CurveDuration = 0.5f; // 혹은 ShrinkCurve->FloatCurve.GetLastKey().Time;
	float StartTime = FMath::Max(0.0f, InDuration - CurveDuration);

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &ATrailWeaponActor::StartShrink, StartTime, false);
}



void ATrailWeaponActor::OnDamageTick()
{
	TArray<AActor*> OverlappingActors;
	OverlapComp->GetOverlappingActors(OverlappingActors);
	float applyDamage = 1.0f;
	for (AActor* Actor : OverlappingActors)
	{
		//Enemy 태그를 가진 적만 데미지주기
		if (Actor && Actor->ActorHasTag("Enemy"))
		{
			bool bIsCrit = FMath::RandRange(0.0f, 1.0f) <= WeaponFinalCriticalChance;

			if (bIsCrit) {
				applyDamage = WeaponFinalCriticalDamage;
			}
			else {
				applyDamage = WeaponFinalDamage;
			}


			UE_LOG(LogTemp, Warning, TEXT("OnDamageTick : %s"), *Actor->GetName());
			UGameplayStatics::ApplyDamage(Actor, applyDamage, nullptr, this, UDamageType::StaticClass());
		}
	}
}

void ATrailWeaponActor::OnPoolActivate_Implementation()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void ATrailWeaponActor::OnPoolDeactivate_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	// 1. 모든 타이머 정지 (DamageTimer, ShrinkTimer 등)
	GetWorldTimerManager().ClearAllTimersForObject(this);

	// 2. 타임라인 정지
	if (ScaleTimeline)
	{
		ScaleTimeline->Stop();
	}

}

void ATrailWeaponActor::LifeSpanExpired()
{
	if (UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
	{
		Pool->ReturnToPool(this);
	}
	else
	{
		Super::LifeSpanExpired();
	}
}

// Called when the game starts or when spawned
void ATrailWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	// [중요] 타임라인과 함수 연결 (바인딩)
	if (ScaleCurve)
	{
		// 1. 업데이트 함수 연결 (매 프레임 호출)
		UpdateFunctionFloat.BindUFunction(this, FName("OnShrinkUpdate"));
		// 2. 종료 함수 연결 (끝나면 호출)
		FinishedFunctionEvent.BindUFunction(this, FName("OnShrinkFinished"));

		// 3. 타임라인에 등록
		ScaleTimeline->AddInterpFloat(ScaleCurve, UpdateFunctionFloat);
		ScaleTimeline->SetTimelineFinishedFunc(FinishedFunctionEvent);

		// 루프 안 함
		ScaleTimeline->SetLooping(false);
	}

}

void ATrailWeaponActor::OnShrinkUpdate(float Value)
{
	// 원래 크기 * 커브 값
	SetActorScale3D(InitialScale * Value);
}

void ATrailWeaponActor::OnShrinkFinished()
{
	if (UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
	{
		Pool->ReturnToPool(this);
	}
	else
	{
		Destroy();
	}
}

void ATrailWeaponActor::StartShrink()
{
	// 타임라인 재생!
	if (ScaleTimeline)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartShrink"));
		ScaleTimeline->PlayFromStart();
	}
}

