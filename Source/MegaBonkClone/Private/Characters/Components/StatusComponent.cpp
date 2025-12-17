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
	//1.0x 배율기준

	float PlayerDmgFactor = PlayerDamage;
	float VisionDmgFactor = 1.0f + (0.01f * VisionDamage);
	float ShrineDmgFactor = 1.0f + (0.01f * ShrineDamage);

	ResultDamage = PlayerDmgFactor * VisionDmgFactor * ShrineDmgFactor;
	UE_LOG(LogTemp, Warning, TEXT("ResultDamage: %.1f"), ResultDamage);
	

	// [치명타 확률] (단순 합산)
	ResultCriticalChance = (PlayerCriticalChance + VisionCriticalChance + ShrineCriticalChance)*100.0f;

	// [치명타 대미지 배율 (Crit Dmg Rate)]
	// 공식 n: (0.01 * Player) * (1 + (0.001 * Shrine))
	// * 무기 대미지 y와 상수 x는 실제 피격 시 계산하므로 여기선 배율 n만 계산합니다.
	float PlayerCritFactor = 0.01f * PlayerCritDmgRate;
	float ShrineCritFactor = 1.0f + (0.001f * ShrineCritDmgRate);

	ResultCritDmgRate = PlayerCritFactor * ShrineCritFactor * 100.0f;


	// [공격 속도] (단순 합산)
	ResultAttackSpeed = (PlayerAttackSpeed + VisionAttackSpeed + ShrineAttackSpeed)*100.0f;


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
	ResultKnockBack = KnockbackPlayerFactor * KnockbackOtherFactor * 100.0f;


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
	ResultProjectileSpeed = (0.01f * PlayerProjectileSpeed) * ProjSpeedOtherFactor * 100.0f;

	float DurationOtherFactor = 1.0f + (0.001f * VisionAttackDuration) + (0.001f * ShrineAttackDuration);
	ResultAttackDuration = (0.01f * PlayerAttackDuration) * DurationOtherFactor * 100.0f;


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
	

		float MoveN = ((0.01f * PlayerMoveSpeed) + (0.001f * VisionMoveSpeed) + (0.01f * ShrineMoveSpeed))*100.0f;

		// 기본 걷기 속도 (예: 400)
		float DefaultWalkSpeed = 400.0f;

		// 공식 해석: n이 배율이므로 기본 속도 * n (예: 1.45배)
		ResultMoveSpeed = DefaultWalkSpeed * MoveN;
		Movement->MaxWalkSpeed = ResultMoveSpeed;
		//UE_LOG(LogTemp, Warning, TEXT("%.1f"), PlayerMoveSpeed);
		//UE_LOG(LogTemp, Warning, TEXT("%.1f , %.1f : %.1f"), DefaultWalkSpeed, MoveN, ResultMoveSpeed);


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

	//UI 델리게이트 갱신용 BroadCast
	//

    //디버그용 프린트 
	if (GEngine)
	{
		FString DebugMsg = FString::Printf(TEXT(
			"===== [Status Debug] =====\n"
			"[생존]\n"
			" MaxHP: %.1f | Regen: %.2f | Shield: %.1f\n"
			" Armor: %.1f%% | Evasion: %.1f%%\n"
			" LifeDrain: %.1f | Thorn: %.1f | OverHeal: %.1f\n"
			"\n"
			"[공격]\n"
			" Damage: %.2fx | CritChance: %.1f%%\n"
			" CritDmg: %.2fx | AtkSpeed: %.1f%%\n"
			" EliteDmg: %.2fx | KnockBack: %.2fx\n"
			"\n"
			"[발사체]\n"
			" Count: %.1f | Reflect: %.1f\n"
			" Size: %.2fx | Speed: %.2fx | Duration: %.2fx\n"
			"\n"
			"[이동]\n"
			" MoveSpeed: %.1f (UnrealUnit)\n"
			" JumpPower: %.1f | ExtraJump: %.1f\n"
			"\n"
			"[유틸리티]\n"
			" Luck: %.1f | Difficulty: %.1f | Range: %.1f\n"
			" Exp: %.2fx | Gold: %.2fx | Silver: %.2fx\n"
			" PowerUpRate: %.2fx | DropRate: %.1f%%"
		),
			// [Survival]
			ResultMaxHP,
			ResultHPRegen,
			ResultShield,
			ResultArmor * 100.0f,         // 0.4 -> 40.0% 표기
			ResultEvasionChance * 100.0f, // 0.1 -> 10.0% 표기
			ResultLifeDrain,
			ResultThorn,
			ResultOverHeal,

			// [Offense]
			ResultDamage,
			ResultCriticalChance,
			ResultCritDmgRate,
			ResultAttackSpeed,
			ResultEliteDamage,
			ResultKnockBack,

			// [Projectile]
			ResultProjectileCount,
			ResultProjectileReflectCount,
			ResultAttackSize,
			ResultProjectileSpeed,
			ResultAttackDuration,

			// [Movement]
			ResultMoveSpeed,
			ResultJumpPower,
			ResultExtraJump,

			// [Utility]
			ResultLuck,
			ResultDifficulty,
			ResultPickUpRange,
			ResultExpGain,
			ResultGoldGain,
			ResultSilverGain,
			ResultPowerUPRate,
			ResultPowerUPDropRate
		);

		// 메시지가 너무 길어서 화면 밖으로 나갈 수 있으므로 Key를 1로 고정하여 갱신
		// 지속시간(Duration)을 조금 길게(10초) 잡아서 읽을 수 있게 함
		GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Green, DebugMsg);
	}
  
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

void UStatusComponent::Debug_TestAllStats()
{
	if (!GEngine) return;

	float LogTime = 15.0f; // 로그 표시 시간 15초

	UE_LOG(LogTemp, Warning, TEXT("========================================="));
	UE_LOG(LogTemp, Warning, TEXT("       [StatusComponent] FULL TEST       "));
	UE_LOG(LogTemp, Warning, TEXT("========================================="));
	GEngine->AddOnScreenDebugMessage(-1, LogTime, FColor::Cyan, TEXT(">>> 모든 스탯 테스트 시작..."));

	// 1. 변경 전 값 기록 (Snapshot)
	// ---------------------------------------------------
	float OldHP = GetResultMaxHP();
	float OldRegen = GetResultHPRegen();
	float OldShield = GetResultShield();
	float OldArmor = GetResultArmor();
	float OldEvasion = GetResultEvasionChance();
	float OldDrain = GetResultLifeDrain();
	float OldThorn = GetResultThorn();

	float OldDmg = GetResultDamage();
	float OldCrit = GetResultCriticalChance();
	float OldCritDmg = GetResultCritDmgRate();
	float OldAtkSpd = GetResultAttackSpeed();
	float OldElite = GetResultEliteDamage();
	float OldKnock = GetResultKnockBack();

	float OldProjCnt = GetResultProjectileCount();
	float OldReflect = GetResultProjectileReflectCount();
	float OldSize = GetResultAttackSize();
	float OldProjSpd = GetResultProjectileSpeed();
	float OldDur = GetResultAttackDuration();

	float OldMove = GetResultMoveSpeed();
	float OldJump = GetResultJumpPower();
	float OldExtraJump = GetResultExtraJump();

	float OldLuck = GetResultLuck();
	float OldPick = GetResultPickUpRange();
	float OldExpG = GetResultExpGain();
	float OldGoldG = GetResultGoldGain();

	// 2. 능력치 강제 주입 (테스트 값 적용)
	// ---------------------------------------------------
	// [생존]
	AddVisionMaxHP(100.0f);        // MaxHP +100
	AddVisionHPRegen(60.0f);       // Regen +60 (분당 60 -> 초당 +1)
	AddShrineOverHeal(50.0f);      // OverHeal +50
	AddVisionShield(30.0f);        // Shield +30
	AddShrineArmor(20.0f);         // Armor +20 (공식: 0.2 / 0.95 = 21%)
	AddVisionEvasionChance(10.0f); // Evasion +10 (공식: 0.1 / 1.1 = 9%)
	AddVisionLifeDrain(5.0f);      // Drain +5
	AddShrineThorn(10.0f);         // Thorn +10

	// [공격]
	AddVisionDamage(50.0f);        // Damage +50% (1.5배)
	AddShrineCriticalChance(0.2f); // CritChance +20% (0.2)
	AddShrineCritDmgRate(500.0f);  // CritDmg +50% (공식: 1 + 0.001*500 = 1.5배)
	AddVisionAttackSpeed(0.5f);    // AtkSpeed +50% (단순 합산 0.5)
	AddShrineEliteDamage(50.0f);   // EliteDmg +50% (1.5배)
	AddVisionKnockBack(1000.0f);   // KnockBack +100% (공식: 1 + 0.001*1000 = 2.0배)

	// [발사체]
	AddShrineProjectileCount(2.0f);       // ProjCount +2
	AddVisionProjectileReflectCount(1.0f); // Reflect +1
	AddVisionAttackSize(500.0f);          // Size +50% (공식: 1 + 0.001*500 = 1.5배)
	AddShrineProjectileSpeed(200.0f);     // Spd +20% (공식: 1 + 0.001*200 = 1.2배)
	AddVisionAttackDuration(300.0f);      // Dur +30% (공식: 1 + 0.001*300 = 1.3배)

	// [이동]
	// 공식상 1 Vision = 0.1 MoveN (+10% 이속), 1 Shrine = 1.0 MoveN (+100% 이속)
	// 테스트를 위해 적당히 넣음
	AddVisionMoveSpeed(5.0f);      // Vision 5 -> +50% 이속 증가 (MoveN += 0.5)
	AddShrineJumpPower(20.0f);     // Jump +20% (1.2배)
	AddVisionExtraJump(1.0f);      // ExtraJump +1

	// [유틸리티]
	AddVisionLuck(10.0f);          // Luck +10
	AddVisionPickUpRange(50.0f);   // PickRange +50% (1.5배)
	AddVisionExpGain(100.0f);      // ExpGain +100% (2.0배)
	AddShrineGoldGain(50.0f);      // GoldGain +50% (1.5배)

	// [레벨업 테스트]
	AddExp(500.0f);                // 경험치 주입

	// 3. 결과 로그 출력 (Old -> New)
	// ---------------------------------------------------
	auto LogStat = [&](const FString& Name, float Old, float New) {
		float Diff = New - Old;
		FString Sign = (Diff >= 0) ? TEXT("+") : TEXT("");
		UE_LOG(LogTemp, Log, TEXT("%-15s : %6.1f -> %6.1f (%s%.1f)"), *Name, Old, New, *Sign, Diff);
		};

	UE_LOG(LogTemp, Warning, TEXT("----- [1. 생존 결과] -----"));
	LogStat("MaxHP", OldHP, GetResultMaxHP());
	LogStat("HPRegen", OldRegen, GetResultHPRegen());
	LogStat("Shield", OldShield, GetResultShield());
	LogStat("Armor(%)", OldArmor * 100.f, GetResultArmor() * 100.f);
	LogStat("Evasion(%)", OldEvasion * 100.f, GetResultEvasionChance() * 100.f);
	LogStat("LifeDrain", OldDrain, GetResultLifeDrain());
	LogStat("Thorn", OldThorn, GetResultThorn());

	UE_LOG(LogTemp, Warning, TEXT("----- [2. 공격 결과] -----"));
	LogStat("Damage", OldDmg, GetResultDamage()/100.0f);
	LogStat("CritChance(%)", OldCrit, GetResultCriticalChance()); // 코드상 이미 *100 되어있음
	LogStat("CritDmg", OldCritDmg, GetResultCritDmgRate());
	LogStat("AtkSpeed(%)", OldAtkSpd, GetResultAttackSpeed());
	LogStat("EliteDmg", OldElite, GetResultEliteDamage());
	LogStat("KnockBack", OldKnock, GetResultKnockBack());

	UE_LOG(LogTemp, Warning, TEXT("----- [3. 발사체 결과] -----"));
	LogStat("ProjCount", OldProjCnt, GetResultProjectileCount());
	LogStat("Reflect", OldReflect, GetResultProjectileReflectCount());
	LogStat("Size", OldSize, GetResultAttackSize());
	LogStat("ProjSpeed", OldProjSpd, GetResultProjectileSpeed());
	LogStat("Duration", OldDur, GetResultAttackDuration());

	UE_LOG(LogTemp, Warning, TEXT("----- [4. 이동/유틸 결과] -----"));
	LogStat("MoveSpeed", OldMove, GetResultMoveSpeed());
	LogStat("JumpPower", OldJump, GetResultJumpPower());
	LogStat("ExtraJump", OldExtraJump, GetResultExtraJump());
	LogStat("Luck", OldLuck, GetResultLuck());
	LogStat("PickRange", OldPick, GetResultPickUpRange());
	LogStat("ExpGain", OldExpG, GetResultExpGain());

	// 화면 메시지
	GEngine->AddOnScreenDebugMessage(-1, LogTime, FColor::Green, TEXT(">>> 테스트 완료! 로그 창을 확인하세요."));
}



// -----------------------------------------------------------------
// [생존 (Survival)]
// -----------------------------------------------------------------
void UStatusComponent::AddVisionMaxHP(float Amount)
{
	VisionMaxHP += Amount;
	if (Amount > 0.0f) CurrentHP += Amount; // 늘어난 만큼 현재 체력도 회복
	UpdateCharacterStatus();
}
void UStatusComponent::AddShrineMaxHP(float Amount)
{
	ShrineMaxHP += Amount;
	if (Amount > 0.0f) CurrentHP += Amount;
	UpdateCharacterStatus();
}

void UStatusComponent::AddCurrentHP(float Amount)
{
	// 현재 체력 변경 (0 ~ 최대 체력 사이로 제한)
	// Amount가 양수면 회복, 음수면 데미지
	CurrentHP = FMath::Clamp(CurrentHP + Amount, 0.0f, ResultMaxHP);

	if (Amount < 0.0f)
	{
		// (선택) 피격 효과나 UI 깜빡임 처리 델리게이트 호출
	}
}

void UStatusComponent::AddGold(float Amount)
{
	CurrentGold += Amount;
	// (선택) 돈이 0보다 작아지지 않게 하려면: FMath::Max(0.0f, CurrentGold + Amount);
	UE_LOG(LogTemp, Log, TEXT("골드 변경: %.0f -> 현재: %.0f"), Amount, CurrentGold);
}

void UStatusComponent::AddExp(float Amount)
{
	//경험치 획득 (획득량 배율 ResultExpGain 적용)
	float FinalExp = Amount * ResultExpGain;

	CurrentExp += FinalExp;
	UE_LOG(LogTemp, Log, TEXT("[Exp] 획득: +%.1f (현재: %.1f / 목표: %.1f)"), FinalExp, CurrentExp, MaxExp);

	//레벨업 체크 (한 번에 여러 레벨이 오를 수 있으므로 while 사용)
	bool bLeveledUp = false;

	while (CurrentExp >= MaxExp)
	{
		//경험치 차감 (통 비우기)
		CurrentExp -= MaxExp;

		//레벨 증가
		CurrentLevel++;

		//다음 레벨 필요 경험치 증가 (성장 배율 적용)
		MaxExp *= ExpGrowthRate;

		bLeveledUp = true;
		UE_LOG(LogTemp, Warning, TEXT("★★★ [Exp] LEVEL UP! Lv.%d (Next Exp: %.1f) ★★★"), CurrentLevel, MaxExp);
	}

	// 3. [Exp 관련] 레벨업 후처리
	if (bLeveledUp)
	{
		//게임멈추고
		//레벨업시 아이템 랜덤 선택창 구현

		//스탯 재계산 (레벨 비례 스탯이 있다면 필수)
		UpdateCharacterStatus();

		//레벨업 델리게이트 호출 (UI, 효과음 등)
		// OnLevelUp.Broadcast(CurrentLevel);
	}
}


void UStatusComponent::AddVisionHPRegen(float Amount) { VisionHPRegen += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineHPRegen(float Amount) { ShrineHPRegen += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionOverHeal(float Amount) { VisionOverHeal += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineOverHeal(float Amount) { ShrineOverHeal += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionShield(float Amount) { VisionShield += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineShield(float Amount) { ShrineShield += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionArmor(float Amount) { VisionArmor += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineArmor(float Amount) { ShrineArmor += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionEvasionChance(float Amount) { VisionEvasionChance += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineEvasionChance(float Amount) { ShrineEvasionChance += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionLifeDrain(float Amount) { VisionLifeDrain += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineLifeDrain(float Amount) { ShrineLifeDrain += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionThorn(float Amount) { VisionThorn += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineThorn(float Amount) { ShrineThorn += Amount; UpdateCharacterStatus(); }


// -----------------------------------------------------------------
// [공격 (Offense)]
// -----------------------------------------------------------------
void UStatusComponent::AddVisionDamage(float Amount) { VisionDamage += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineDamage(float Amount) { ShrineDamage += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionCriticalChance(float Amount) { VisionCriticalChance += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineCriticalChance(float Amount) { ShrineCriticalChance += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionCritDmgRate(float Amount) { VisionCritDmgRate += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineCritDmgRate(float Amount) { ShrineCritDmgRate += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionAttackSpeed(float Amount) { VisionAttackSpeed += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineAttackSpeed(float Amount) { ShrineAttackSpeed += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionEliteDamage(float Amount) { VisionEliteDamage += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineEliteDamage(float Amount) { ShrineEliteDamage += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionKnockBack(float Amount) { VisionKnockBack += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineKnockBack(float Amount) { ShrineKnockBack += Amount; UpdateCharacterStatus(); }


// -----------------------------------------------------------------
// [발사체 (Projectile)]
// -----------------------------------------------------------------
void UStatusComponent::AddVisionProjectileCount(float Amount) { VisionProjectileCount += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineProjectileCount(float Amount) { ShrineProjectileCount += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionProjectileReflectCount(float Amount) { VisionProjectileReflectCount += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineProjectileReflectCount(float Amount) { ShrineProjectileReflectCount += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionAttackSize(float Amount) { VisionAttackSize += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineAttackSize(float Amount) { ShrineAttackSize += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionProjectileSpeed(float Amount) { VisionProjectileSpeed += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineProjectileSpeed(float Amount) { ShrineProjectileSpeed += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionAttackDuration(float Amount) { VisionAttackDuration += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineAttackDuration(float Amount) { ShrineAttackDuration += Amount; UpdateCharacterStatus(); }


// -----------------------------------------------------------------
// [이동 (Movement)]
// -----------------------------------------------------------------
void UStatusComponent::AddVisionMoveSpeed(float Amount) { VisionMoveSpeed += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineMoveSpeed(float Amount) { ShrineMoveSpeed += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionExtraJump(float Amount) { VisionExtraJump += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineExtraJump(float Amount) { ShrineExtraJump += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionJumpPower(float Amount) { VisionJumpPower += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineJumpPower(float Amount) { ShrineJumpPower += Amount; UpdateCharacterStatus(); }


// -----------------------------------------------------------------
// [유틸리티 (Utility)]
// -----------------------------------------------------------------
void UStatusComponent::AddVisionLuck(float Amount) { VisionLuck += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineLuck(float Amount) { ShrineLuck += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionDifficulty(float Amount) { VisionDifficulty += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineDifficulty(float Amount) { ShrineDifficulty += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionPickUpRange(float Amount) { VisionPickUpRange += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrinePickUpRange(float Amount) { ShrinePickUpRange += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionExpGain(float Amount) { VisionExpGain += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineExpGain(float Amount) { ShrineExpGain += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionGoldGain(float Amount) { VisionGoldGain += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineGoldGain(float Amount) { ShrineGoldGain += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionSilverGain(float Amount) { VisionSilverGain += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrineSilverGain(float Amount) { ShrineSilverGain += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionPowerUPRate(float Amount) { VisionPowerUPRate += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrinePowerUPRate(float Amount) { ShrinePowerUPRate += Amount; UpdateCharacterStatus(); }

void UStatusComponent::AddVisionPowerUPDropRate(float Amount) { VisionPowerUPDropRate += Amount; UpdateCharacterStatus(); }
void UStatusComponent::AddShrinePowerUPDropRate(float Amount) { ShrinePowerUPDropRate += Amount; UpdateCharacterStatus(); }

