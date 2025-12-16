// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/TrailWeaponActor.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATrailWeaponActor::ATrailWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	OverlapComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("OverlapComp"));
	OverlapComp->SetupAttachment(Root);
	OverlapComp->SetCapsuleHalfHeight(100.0f);
	OverlapComp->SetCapsuleRadius(100.0f);
	OverlapComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	EffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	EffectComp->SetupAttachment(Root);

}

void ATrailWeaponActor::InitializeTrail(float InDamage, float InDuration, float InScale, float AttackSpeed)
{
	Damage = InDamage;
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
		AttackSpeed,
		true
	);
}



void ATrailWeaponActor::OnDamageTick()
{
	TArray<AActor*> OverlappingActors;
	OverlapComp->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		//Enemy 태그를 가진 적만 데미지주기
		if (Actor && Actor->ActorHasTag("Enemy"))
		{
			UE_LOG(LogTemp, Warning, TEXT("OnDamageTick : %s"), *Actor->GetName());
			UGameplayStatics::ApplyDamage(Actor, Damage, nullptr, this, UDamageType::StaticClass());
		}
	}
}

// Called when the game starts or when spawned
void ATrailWeaponActor::BeginPlay()
{
	Super::BeginPlay();

}

