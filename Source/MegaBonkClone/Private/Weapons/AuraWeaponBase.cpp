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

	// 이 함수는 타이머에 의해 '공격 속도' 주기로 계속 불립니다.
	// 즉, 0.5초마다 불리면 0.5초마다 범위 내 적에게 데미지를 줍니다.
	if (Implements<UWeapon>()) {
		IWeapon::Execute_GetDamageWeapon(this);
	}

	TArray<AActor*> OverlappingActors;
	Collision->GetOverlappingActors(OverlappingActors);
	float DamageToApply;
	if (CheckIsCritical()) {
		DamageToApply = WeaponFinalCriticalDamage;
	}
	else {
		DamageToApply= WeaponFinalDamage;
	}

	

	// 3. 반복문으로 데미지 적용
	bool bHitAny = false;
	for (AActor* Actor : OverlappingActors)
	{
		// 유효한 타겟인지 확인 (WeaponBase에 있는 함수 활용)
		if (IsValidTarget(Actor))
		{
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
	// 플레이어의 범위 증가 스탯(AttackSize) 반영
	float BaseScale = 5.0f;
	float PlayerBonus = 1.0f;
	if (OwnerStatusComp.IsValid())
	{// StatusComponent의 공격 범위 스탯
		PlayerBonus = OwnerStatusComp->GetResultAttackSize(); 
	}

	// 최종 크기 적용
	float FinalScale = PlayerBonus * BaseScale;
	SetActorScale3D(FVector(FinalScale));
}



// Called when the game starts or when spawned
void AAuraWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	//오라 크기 설정
	UpdateAuraScale();
}


