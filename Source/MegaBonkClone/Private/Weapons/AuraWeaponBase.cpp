// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AuraWeaponBase.h"
#include "Characters/Components/StatusComponent.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAuraWeaponBase::AAuraWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	

	BaseRoot = CreateDefaultSubobject<USceneComponent>("BaseRoot");
	SetRootComponent(BaseRoot);

	BaseSprite = CreateDefaultSubobject<UPaperSpriteComponent>("BaseSprite");
	BaseSprite->SetupAttachment(BaseRoot);
	BaseSprite->SetCollisionProfileName("NoCollision");
	BaseSprite->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	BaseSprite->SetSpriteColor(FColor::FromHex("66CCFF"));
	
	Collision = CreateDefaultSubobject<UCapsuleComponent>("Collision");
	Collision->SetupAttachment(BaseRoot); 
	Collision->SetCapsuleSize(35.0f, 35.0f);
	
}

void AAuraWeaponBase::AttackWeapon_Implementation()
{
	Super::AttackWeapon_Implementation();

	UpdateAuraScale();

	TArray<AActor*> OverlappingActors;
	Collision->GetOverlappingActors(OverlappingActors);

	// 3. 반복문으로 데미지 적용
	bool bHitAny = false;
	for (AActor* Actor : OverlappingActors)
	{
		// 유효한 타겟인지 확인 (WeaponBase에 있는 함수 활용)
		if (IsValidTarget(Actor))
		{
			float DamageToApply;
			if (CheckIsCritical()) {
				DamageToApply = WeaponFinalCriticalDamage;
			}
			else {
				DamageToApply = WeaponFinalDamage;
			}
			// 데미지 전달
			UGameplayStatics::ApplyDamage(
				Actor,
				DamageToApply,
				GetInstigatorController(), // 혹은 nullptr
				this,
				UDamageType::StaticClass()
			);
			bHitAny = true;

			if (bHitAny)
			{
				//UE_LOG(LogTemp, Log, TEXT("Aura Hit! %s에 Damage: %f"), *Actor->GetName(), DamageToApply);
				// (선택) 타격감이 필요하면 여기서 사운드나 이펙트 재생
			}

		}
	}

	
}

void AAuraWeaponBase::UpdateAuraScale()
{
	float auraBaseSize = 5.0f;
	SetActorScale3D(FVector(FinalAttackSize * auraBaseSize));
}

void AAuraWeaponBase::UpdateWeaponStats()
{
	Super::UpdateWeaponStats();

	//UpdateAuraScale();
}



// Called when the game starts or when spawned
void AAuraWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	//오라 크기 설정
	UpdateAuraScale();
}


