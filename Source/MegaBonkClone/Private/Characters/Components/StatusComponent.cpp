// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/StatusComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h" // 수학 함수용
// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    
	// ...
}


// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
    Movement = OwnerCharacter->GetCharacterMovement();


    //캐릭터 존재 확인용 로그
    /*if (OwnerCharacter && Movement) {
        UE_LOG(LogTemp, Warning, TEXT("UStatusComponent : 캐릭터 존재"));
    }*/
}

void UStatusComponent::UpdateCharacterStatus()
{
    // 0. 주인 캐릭터와 무브먼트 컴포넌트 가져오기
    if (!OwnerCharacter) return;
    if (!Movement) return;

    // ==========================================================
    // 1. 계산 로직 (수식 적용)
    // ==========================================================

    // [방어력] (감소율 공식: 0~1 사이 값)
    // Armor가 입력 변수이므로, 결과는 로컬 변수로 쓰거나 별도 Result 변수가 필요합니다.
    float CalculatedArmorReduction = (0.01f * Armor) / (0.75f + (0.01f * Armor));

    // [회피율]
    float CalculatedEvasion = (0.01f * EvasionChance) / (1.0f + (0.01f * EvasionChance));

    // [기타 배율들]
    float ExpMultiplier = (1.0f + (0.01f * ExpGain));
    float GoldMultiplier = (1.0f + (0.01f * GoldGain));
    float SilverMultiplier = (1.0f + (0.01f * SilverGain));
    float PickupRangeMultiplier = (1.0f + (0.01f * PickUpRange));

    // [크리티컬 데미지] (Result 변수가 존재함 -> 셋터 사용 가능)
    // X: 무기 치피, Y: 무기 데미지라고 가정
    float WeaponCritDmg = 1.5f; // 예시값
    float CalculatedCritDmgRate = (1.0f + WeaponCritDmg) * (0.01f * PlayerCritDmgRate) * (1.0f + (0.001f * ShrineCritDmgRate));

    // ✅ 셋터 사용: 결과 저장
    SetResultCritDmgRate(CalculatedCritDmgRate);


    // [공격 크기]
    float CalculatedAttackSize = (1.0f + (0.001f * AttackSize));


    // [투사체 속도] (Result 변수가 존재함 -> 셋터 사용 가능)
    float WeaponBaseSpeed = 1000.0f; // 예시: 5 대신 실제 속도 단위 사용
    float CalculatedProjectileSpeed = WeaponBaseSpeed * (0.01f * PlayerProjectileSpeed) * (1.0f + (0.001f * VisionProjectileSpeed) + (0.001f * ShrineProjectileSpeed));

    // ✅ 셋터 사용: 결과 저장
    SetResultProjectileSpeed(CalculatedProjectileSpeed);


    // [공격 지속 시간]
    float WeaponBaseDuration = 5.0f;
    float CalculatedDuration = WeaponBaseDuration * (0.001f * AttackDuration);


    // [넉백]
    float WeaponBaseKnockback = 500.0f;
    float CalculatedKnockback = WeaponBaseKnockback * (0.001f * KnockBack);


    // ==========================================================
    // 2. 물리/이동 관련 적용 (CharacterMovement)
    // ==========================================================

    if (Movement)
    {
        // [이동 속도]
        // n: 캐릭터의 기본 속도 (예: 600)
        float BaseWalkSpeed = 400.0f;

        // MoveSpeed가 % 증가량(예: 10 => 10% 증가)이라고 가정했을 때의 수식
        float FinalMoveSpeed = BaseWalkSpeed * (1.0f + (0.01f * MoveSpeed));

        // 실제 캐릭터 속도 변경
        Movement->MaxWalkSpeed = FinalMoveSpeed;


        // [점프력]
        // n: 캐릭터 기본 점프력 (예: 420)
        float BaseJumpZ = 350.0f;

        // JumpPower가 % 증가량이라고 가정
        float FinalJumpPower = BaseJumpZ * (1.0f + (0.01f * JumpPower));

        // 실제 캐릭터 점프력 변경
        Movement->JumpZVelocity = FinalJumpPower;

        // [추가 점프 횟수] (소수점 버림 처리)
        // ExtraJump가 float이므로 int로 변환
        int32 FinalExtraJumps = FMath::FloorToInt(ExtraJump);
        OwnerCharacter->JumpMaxCount = 1 + FinalExtraJumps; // 기본 1 + 추가점프
    }

    //디버그용 프린트 
    if (GEngine)
    {
        FString DebugMsg = FString::Printf(TEXT(
            "--- Status Update ---\n"
            "Final MoveSpeed: %f\n"
            "Final JumpZ: %f\n"
            "Result CritDmg: %f\n"
            "Result ProjSpeed: %f"
        ),
            Movement ? Movement->MaxWalkSpeed : 0.0f, // 실제 적용된 이동속도
            Movement ? Movement->JumpZVelocity : 0.0f, // 실제 적용된 점프력
            ResultCritDmgRate,
            ResultProjectileSpeed
        );

        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, DebugMsg);
    }
}

float UStatusComponent::GetStatusDamage()
{

    // [데미지] (기본 데미지 * 배율)
    // 주의: 무기 데미지를 가져오는 로직이 필요합니다. 여기선 예시로 10.0f라 가정합니다.
    float FinalDamage = (1.0f + (0.01f * Damage));

    // [엘리트 데미지]
    float CalculatedEliteDamage = FinalDamage * (1.0f + (0.01f * EliteDamage));

    return FinalDamage;

    //엘리트면
    //return CalculatedEliteDamage;
}




