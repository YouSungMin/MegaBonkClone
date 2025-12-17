// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/StatusComponent.h"
#include "Data/CharacterDataStruct.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h" 
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

    
    //initialize에서 처리중 확인되면 지워주세요
    //CurrentHP = MaxHP;

    //캐릭터 존재 확인용 로그
    /*if (OwnerCharacter && Movement) {
        UE_LOG(LogTemp, Warning, TEXT("UStatusComponent : 캐릭터 존재"));
    }*/
    
}

void UStatusComponent::UpdateCharacterStatus()
{
	// 0. 주인 캐릭터와 컴포넌트 유효성 확인
	if (!OwnerCharacter) return;
	// Movement가 없을 수도 있으므로 체크 (생성자에서 가져오거나 BeginPlay에서 설정됨)
	if (!Movement) Movement = OwnerCharacter->GetCharacterMovement();


	// ==========================================================
	// 1. 생존 (Survival)
	// ==========================================================

	// [최대 HP] (단순 합산)
	ResultMaxHP = PlayerMaxHP + VisionMaxHP + ShrineMaxHP;

	// HP 갱신: 만약 현재 체력이 최대 체력보다 높으면(오버힐 제외 상황) 조정이 필요할 수 있으나, 
	// 오버힐 로직은 회복 시점에 처리하므로 여기선 MaxHP 값만 갱신합니다.


	// [HP 회복] (분당 회복량 -> 초당 회복량 변환: / 60)
	// 공식: (Player + Vision + Shrine) / 60
	ResultHPRegen = (PlayerHPRegen + VisionHPRegen + ShrineHPRegen) / 60.0f;


	// [오버힐] (아이템 개수 표시 -> 단순 합산)
	ResultOverHeal = PlayerOverHeal + VisionOverHeal + ShrineOverHeal;


	// [쉴드] (최대치 합산)
	ResultShield = PlayerShield + VisionShield + ShrineShield;


	// [방어력 (Armor)]
	// 공식: n / (0.75 + n)
	// n = (0.01 * Player) + (0.01 * Vision) + (0.01 * Shrine)
	float ArmorN = (0.01f * PlayerArmor) + (0.01f * VisionArmor) + (0.01f * ShrineArmor);

	if (ArmorN > 0.0f)
	{
		ResultArmor = ArmorN / (0.75f + ArmorN);
	}
	else
	{
		ResultArmor = 0.0f;
	}


	// [회피 (Evasion)]
	// 공식: n / (1 + n)
	// n = (0.01 * Player) + (0.01 * Vision) + (0.01 * Shrine)
	float EvasionN = (0.01f * PlayerEvasionChance) + (0.01f * VisionEvasionChance) + (0.01f * ShrineEvasionChance);

	if (EvasionN > 0.0f)
	{
		ResultEvasionChance = EvasionN / (1.0f + EvasionN);
	}
	else
	{
		ResultEvasionChance = 0.0f;
	}


	// [생명 흡수 & 가시] (단순 합산)
	ResultLifeDrain = PlayerLifeDrain + VisionLifeDrain + ShrineLifeDrain;
	ResultThorn = PlayerThorn + VisionThorn + ShrineThorn;


	// ==========================================================
	// 2. 공격 (Offense)
	// ==========================================================

	// [대미지 (Damage)]
	// 공식: (0.01 * Player) * (0.01 * Vision) * (1 + (0.01 * Shrine))
	// 주의: Vision이 0이면 대미지가 0이 되는 것을 방지하기 위해, 0일 경우 100(1.0)으로 처리하거나 
	// 기본적으로 100을 베이스로 가집니다. 여기서는 값이 0이면 100.0f(1배)로 간주합니다.
	float PlayerDmgFactor = 0.01f * PlayerDamage;
	float VisionDmgFactor = (VisionDamage > 0.0f) ? (0.01f * VisionDamage) : 1.0f; // 0이면 1배
	float ShrineDmgFactor = 1.0f + (0.01f * ShrineDamage);

	ResultDamage = PlayerDmgFactor * VisionDmgFactor * ShrineDmgFactor;


	// [치명타 확률] (단순 합산)
	ResultCriticalChance = PlayerCriticalChance + VisionCriticalChance + ShrineCriticalChance;


	// [치명타 대미지 배율 (Crit Dmg Rate)]
	// 공식 n: (0.01 * Player) * (1 + (0.001 * Shrine))
	// * 무기 대미지 y와 상수 x는 실제 피격 시 계산하므로 여기선 배율 n만 계산합니다.
	float PlayerCritFactor = 0.01f * PlayerCritDmgRate;
	float ShrineCritFactor = 1.0f + (0.001f * ShrineCritDmgRate);

	ResultCritDmgRate = PlayerCritFactor * ShrineCritFactor;


	// [공격 속도] (단순 합산)
	ResultAttackSpeed = PlayerAttackSpeed + VisionAttackSpeed + ShrineAttackSpeed;


	// [엘리트 대미지]
	// 공식: 대미지 * n
	// n = (1 + (0.01 * Player) + (0.01 * Shrine))
	// * 여기서는 '대미지' 변수까지 곱한 최종값을 저장하거나, 배율 n만 저장할 수 있습니다.
	//   요청하신 공식 흐름상 "대미지 * n" 이므로 ResultDamage에 배율을 곱해 저장합니다.
	float EliteFactor = 1.0f + (0.01f * PlayerEliteDamage) + (0.01f * ShrineEliteDamage);
	ResultEliteDamage = ResultDamage * EliteFactor;


	// [넉백 (Knockback)]
	// 공식: Base * n (여기선 배율 n을 계산하여 저장)
	// n = (0.01 * Player) * (1 + (0.001 * Vision) + (0.001 * Shrine))
	float KnockbackPlayerFactor = 0.01f * PlayerKnockBack;
	float KnockbackOtherFactor = 1.0f + (0.001f * VisionKnockBack) + (0.001f * ShrineKnockBack);

	// ResultKnockBack은 무기 Base에 곱해질 '배율'로 저장합니다.
	ResultKnockBack = KnockbackPlayerFactor * KnockbackOtherFactor;


	// ==========================================================
	// 3. 발사체 (Projectile)
	// ==========================================================

	// [발사체 수 & 반사] (단순 합산)
	ResultProjectileCount = PlayerProjectileCount + VisionProjectileCount + ShrineProjectileCount;
	ResultProjectileReflectCount = PlayerProjectileReflectCount + VisionProjectileReflectCount + ShrineProjectileReflectCount;


	// [크기 (Size)]
	// 공식: (0.01 * 무기크기) * n
	// n = Player * (1 + (0.001 * Vision) + (0.001 * Shrine))
	// * 여기선 캐릭터 스탯 부분인 n을 계산합니다. PlayerAttackSize는 보통 100(100%) 등으로 가정.
	float SizeOtherFactor = 1.0f + (0.001f * VisionAttackSize) + (0.001f * ShrineAttackSize);

	// 최종적으로 무기에 곱해질 배율 값 (Player가 100이면 100 * 1.xx 가 됨 -> 무기에서 0.01 곱함)
	ResultAttackSize = PlayerAttackSize * SizeOtherFactor;


	// [발사체 속도 & 지속 시간]
	// 공식: n = (0.01 * Player) * (1 + (0.001 * Vision) + (0.001 * Shrine))
	float ProjSpeedOtherFactor = 1.0f + (0.001f * VisionProjectileSpeed) + (0.001f * ShrineProjectileSpeed);
	ResultProjectileSpeed = (0.01f * PlayerProjectileSpeed) * ProjSpeedOtherFactor;

	float DurationOtherFactor = 1.0f + (0.001f * VisionAttackDuration) + (0.001f * ShrineAttackDuration);
	ResultAttackDuration = (0.01f * PlayerAttackDuration) * DurationOtherFactor;


	// ==========================================================
	// 4. 이동 (Movement)
	// ==========================================================

	if (Movement)
	{
		// [이동 속도]
		// 공식: PlayerInitial + n  (PlayerInitial을 기본 배율 1로 보거나, 기본 속도에 곱함)
		// n = (0.01 * Player) + (0.001 * Vision) + (0.01 * Shrine) -> 예시: 0.01*15 -> 0.15
		// * PlayerMoveSpeed가 100(기본)이라면 0.01*100 = 1.0. 
		// * Result는 실제 언리얼 유닛 속도로 변환하여 적용합니다.

		float MoveN = (0.01f * PlayerMoveSpeed) + (0.001f * VisionMoveSpeed) + (0.01f * ShrineMoveSpeed);

		// 기본 걷기 속도 (예: 400)
		float DefaultWalkSpeed = 400.0f;

		// 공식 해석: n이 배율이므로 기본 속도 * n (예: 1.45배)
		ResultMoveSpeed = DefaultWalkSpeed * MoveN;
		Movement->MaxWalkSpeed = ResultMoveSpeed;


		// [점프 높이 (Power)]
		// 공식: Initial * n
		// n = (1 + (0.01 * Player) + (0.01 * Shrine))
		float JumpN = 1.0f + (0.01f * PlayerJumpPower) + (0.01f * ShrineJumpPower);

		float DefaultJumpZ = 420.0f; // 기본 점프력
		ResultJumpPower = DefaultJumpZ * JumpN;
		Movement->JumpZVelocity = ResultJumpPower;
	}

	// [추가 점프] (성소 등에서 정수만큼 추가)
	ResultExtraJump = PlayerExtraJump + VisionExtraJump + ShrineExtraJump;
	if (OwnerCharacter)
	{
		// 기본 점프 1 + 추가 점프 (소수점 버림)
		OwnerCharacter->JumpMaxCount = 1 + FMath::FloorToInt(ResultExtraJump);
	}


	// ==========================================================
	// 5. 유틸리티 (Utility)
	// ==========================================================

	// [행운 & 난이도] (단순 합산)
	ResultLuck = PlayerLuck + VisionLuck + ShrineLuck;
	ResultDifficulty = PlayerDifficulty + VisionDifficulty + ShrineDifficulty;


	// [픽업 범위]
	// 공식: Initial * n
	// n = (1 + (0.01 * Vision) + (0.01 * Shrine))
	// * PlayerPickUpRange가 Initial 역할을 한다고 가정 (예: 10)
	float PickupN = 1.0f + (0.01f * VisionPickUpRange) + (0.01f * ShrinePickUpRange);
	ResultPickUpRange = PlayerPickUpRange * PickupN;

	
	if (UCapsuleComponent* PickupComp =OwnerCharacter->GetCapsuleComponent())
	{
		// 기본 10이 너무 작으므로, 언리얼 단위 보정(예: *10.0f)이 필요할 수 있으나 
		// 여기선 계산된 Result 값을 그대로 적용합니다.
		PickupComp->SetCapsuleRadius(ResultPickUpRange);
	}
	


	// [XP / 골드 / 실버 증가]
	// 공식: Initial * n
	// n = (1 + (0.01 * Vision) + (0.01 * Shrine))
	// * Player 수치를 Initial(기본 배율)로 사용

	float XPN = 1.0f + (0.01f * VisionExpGain) + (0.01f * ShrineExpGain);
	ResultExpGain = PlayerExpGain * XPN;

	float GoldN = 1.0f + (0.01f * VisionGoldGain) + (0.01f * ShrineGoldGain);
	ResultGoldGain = PlayerGoldGain * GoldN;

	float SilverN = 1.0f + (0.01f * VisionSilverGain) + (0.01f * ShrineSilverGain);
	ResultSilverGain = PlayerSilverGain * SilverN;


	// [파워업 관련] (단순 합산)
	ResultPowerUPRate = PlayerPowerUPRate + VisionPowerUPRate + ShrinePowerUPRate;
	ResultPowerUPDropRate = PlayerPowerUPDropRate + VisionPowerUPDropRate + ShrinePowerUPDropRate;

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
    //float FinalDamage = (1.0f + (0.01f * Damage));

    // [엘리트 데미지]
    //float CalculatedEliteDamage = FinalDamage * (1.0f + (0.01f * EliteDamage));

    //return FinalDamage;

    //엘리트면
    //return CalculatedEliteDamage;

	return 0.0f;
}

void UStatusComponent::InitializeStatsFromDataTable(const FDataTableRowHandle& InDataHandle)
{
    // 1. 데이터 테이블이 유효한지 확인
    if (InDataHandle.IsNull())
    {
        UE_LOG(LogTemp, Error, TEXT("InitializeStatsFromDataTable - 데이터 핸들이 비어있습니다."));
        return;
    }

    // 2. GetRow 함수로 바로 데이터 가져오기 (매우 간편!)
    static const FString ContextString(TEXT("StatusComponent Initialization"));
    FCharacterData* RowData = InDataHandle.GetRow<FCharacterData>(ContextString);

    if (RowData)
    {
        UE_LOG(LogTemp, Log, TEXT("캐릭터 데이터 로드 성공: %s"), *InDataHandle.RowName.ToString());
        // ============================================================
        //  메쉬 설정
        // ============================================================
        
        if (!RowData->SkeletalMesh.IsNull())
        {
          
            USkeletalMesh* NewMesh = RowData->SkeletalMesh.LoadSynchronous();
            if (OwnerCharacter && NewMesh)
            {
                OwnerCharacter->GetMesh()->SetSkeletalMesh(NewMesh);
            }
        }

        // ============================================================
        //  생명력 및 방어 관련 (Health & Defense)
        // ============================================================
        PlayerMaxHP = RowData->MaxHP;
        CurrentHP = PlayerMaxHP; // 최대 체력으로 현재 체력 초기화
        PlayerHPRegen = RowData->HPRegen;
        PlayerOverHeal = RowData->OverHeal;
        PlayerShield = RowData->Shield;
        PlayerArmor = RowData->Armor;
        PlayerEvasionChance = RowData->EvasionChance;
        PlayerLifeDrain = RowData->LifeDrain;
        PlayerThorn = RowData->Thorn;

        // ============================================================
        // [공격 (Offense)]
        // ============================================================
        PlayerDamage = RowData->Damage;
        PlayerCriticalChance = RowData->CriticalChance;
        PlayerCritDmgRate = RowData->PlayerCritDmgRate;
        PlayerAttackSpeed = RowData->AttackSpeed;

        // ============================================================
        // [발사체 (Projectile)]
        // ============================================================
        PlayerProjectileCount = RowData->ProjectileCount;
        PlayerProjectileReflectCount = RowData->ProjectileReflectCount;
        PlayerAttackSize = RowData->AttackSize;
        PlayerProjectileSpeed = RowData->PlayerProjectileSpeed;
        PlayerAttackDuration = RowData->AttackDuration;
        PlayerEliteDamage = RowData->EliteDamage;
        PlayerKnockBack = RowData->KnockBack;

        // ============================================================
        // [이동 (Movement)]
        // ============================================================
        PlayerMoveSpeed = RowData->MoveSpeed;
        PlayerExtraJump = RowData->ExtraJump;
        PlayerJumpPower = RowData->JumpPower;

        // ============================================================
        // [유틸리티 (Utility)]
        // ============================================================
        PlayerLuck = RowData->Luck;
        PlayerDifficulty = RowData->Difficulty;
        PlayerPickUpRange = RowData->PickUpRange;
        PlayerExpGain = RowData->ExpGain;
        PlayerGoldGain = RowData->GoldGain;
        PlayerSilverGain = RowData->SilverGain;
        //PlayerEliteSpawnRate = RowData->EliteSpawnRate; // (변수 있다면 주석 해제)
        PlayerPowerUPRate = RowData->PowerUPRate;
        PlayerPowerUPDropRate = RowData->PowerUPDropRate;

        // 2. 값 설정이 끝났으니, 실제 게임 로직(무브먼트 속도 등)에 적용
        UpdateCharacterStatus();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("데이터를 찾을 수 없습니다. (RowName: %s)"), *InDataHandle.RowName.ToString());
    }
}




