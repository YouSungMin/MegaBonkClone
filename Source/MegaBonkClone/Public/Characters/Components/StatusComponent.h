// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

struct FCharacterData;
class UResourceBarWidget; //HP,Shield바


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatusUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEGABONKCLONE_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

public:
	//스탯 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Status|Event")
	FOnStatusUpdated OnStatusUpdated;

public:

	// -----------------------------------------------------------------
	// [생존 (Survival)]
	// -----------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionMaxHP(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineMaxHP(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionHPRegen(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineHPRegen(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionOverHeal(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineOverHeal(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionShield(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineShield(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionArmor(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineArmor(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionEvasionChance(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineEvasionChance(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionLifeDrain(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineLifeDrain(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionThorn(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineThorn(float Amount);

	// -----------------------------------------------------------------
	// [공격 (Offense)]
	// -----------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionDamage(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineDamage(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionCriticalChance(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineCriticalChance(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionCritDmgRate(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineCritDmgRate(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionAttackSpeed(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineAttackSpeed(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionEliteDamage(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineEliteDamage(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionKnockBack(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineKnockBack(float Amount);

	// -----------------------------------------------------------------
	// [발사체 (Projectile)]
	// -----------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionProjectileCount(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineProjectileCount(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionProjectileReflectCount(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineProjectileReflectCount(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionAttackSize(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineAttackSize(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionProjectileSpeed(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineProjectileSpeed(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionAttackDuration(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineAttackDuration(float Amount);

	// -----------------------------------------------------------------
	// [이동 (Movement)]
	// -----------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionMoveSpeed(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineMoveSpeed(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionExtraJump(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineExtraJump(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionJumpPower(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineJumpPower(float Amount);

	// -----------------------------------------------------------------
	// [유틸리티 (Utility)]
	// -----------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionLuck(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineLuck(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionDifficulty(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineDifficulty(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionPickUpRange(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrinePickUpRange(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionExpGain(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineExpGain(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionGoldGain(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineGoldGain(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionSilverGain(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrineSilverGain(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionPowerUPRate(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrinePowerUPRate(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddVisionPowerUPDropRate(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Status|Modify") void AddShrinePowerUPDropRate(float Amount);


	//Getter , Setter 함수

	// =================================================================
	// [상태 변수 (State)]
	// =================================================================
	// HP
	UFUNCTION(BlueprintCallable) inline float GetCurrentHP() const { return CurrentHP; }
	UFUNCTION(BlueprintCallable) void AddCurrentHP(float Amount); // [추가] 힐(+), 데미지(-) 모두 처리

	// Gold
	UFUNCTION(BlueprintCallable) inline float GetCurrentGold() const { return CurrentGold; }
	UFUNCTION(BlueprintCallable) void AddGold(float Amount);  

	// Exp
	UFUNCTION(BlueprintCallable) inline float GetCurrentExp() const { return CurrentExp; }
	UFUNCTION(BlueprintCallable) void AddExp(float Amount);       // [추가]

	UFUNCTION(BlueprintCallable) inline int32 GetCurrentLevel() const { return CurrentLevel; }
	UFUNCTION(BlueprintCallable) inline float GetMaxExp() const { return MaxExp; }

	// =================================================================
	// [생존 (Survival)] - 최종 결과값 Getter만 유지
	// =================================================================

	UFUNCTION(BlueprintCallable) inline float GetResultMaxHP() const { return ResultMaxHP; }
	UFUNCTION(BlueprintCallable) inline float GetResultHPRegen() const { return ResultHPRegen; }
	UFUNCTION(BlueprintCallable) inline float GetResultOverHeal() const { return ResultOverHeal; }
	UFUNCTION(BlueprintCallable) inline float GetResultShield() const { return ResultShield; }
	UFUNCTION(BlueprintCallable) inline float GetResultArmor() const { return ResultArmor; }
	UFUNCTION(BlueprintCallable) inline float GetResultEvasionChance() const { return ResultEvasionChance; }
	UFUNCTION(BlueprintCallable) inline float GetResultLifeDrain() const { return ResultLifeDrain; }
	UFUNCTION(BlueprintCallable) inline float GetResultThorn() const { return ResultThorn; }


	// =================================================================
	// [공격 (Offense)]
	// =================================================================

	UFUNCTION(BlueprintCallable) inline float GetResultDamage() const { return ResultDamage; }
	UFUNCTION(BlueprintCallable) inline float GetResultCriticalChance() const { return ResultCriticalChance; }
	UFUNCTION(BlueprintCallable) inline float GetResultCritDmgRate() const { return ResultCritDmgRate; }
	UFUNCTION(BlueprintCallable) inline float GetResultAttackSpeed() const { return ResultAttackSpeed; }
	UFUNCTION(BlueprintCallable) inline float GetResultEliteDamage() const { return ResultEliteDamage; }
	UFUNCTION(BlueprintCallable) inline float GetResultKnockBack() const { return ResultKnockBack; }


	// =================================================================
	// [발사체 (Projectile)]
	// =================================================================

	UFUNCTION(BlueprintCallable) inline float GetResultProjectileCount() const { return ResultProjectileCount; }
	UFUNCTION(BlueprintCallable) inline float GetResultProjectileReflectCount() const { return ResultProjectileReflectCount; }
	UFUNCTION(BlueprintCallable) inline float GetResultAttackSize() const { return ResultAttackSize; }
	UFUNCTION(BlueprintCallable) inline float GetResultProjectileSpeed() const { return ResultProjectileSpeed; }
	UFUNCTION(BlueprintCallable) inline float GetResultAttackDuration() const { return ResultAttackDuration; }


	// =================================================================
	// [이동 (Movement)]
	// =================================================================

	UFUNCTION(BlueprintCallable) inline float GetResultMoveSpeed() const { return ResultMoveSpeed; }
	UFUNCTION(BlueprintCallable) inline float GetResultExtraJump() const { return ResultExtraJump; }
	UFUNCTION(BlueprintCallable) inline float GetResultJumpPower() const { return ResultJumpPower; }


	// =================================================================
	// [유틸리티 (Utility)]
	// =================================================================

	UFUNCTION(BlueprintCallable) inline float GetResultLuck() const { return ResultLuck; }
	UFUNCTION(BlueprintCallable) inline float GetResultDifficulty() const { return ResultDifficulty; }
	UFUNCTION(BlueprintCallable) inline float GetResultPickUpRange() const { return ResultPickUpRange; }
	UFUNCTION(BlueprintCallable) inline float GetResultExpGain() const { return ResultExpGain; }
	UFUNCTION(BlueprintCallable) inline float GetResultGoldGain() const { return ResultGoldGain; }
	UFUNCTION(BlueprintCallable) inline float GetResultSilverGain() const { return ResultSilverGain; }
	UFUNCTION(BlueprintCallable) inline float GetResultPowerUPRate() const { return ResultPowerUPRate; }
	UFUNCTION(BlueprintCallable) inline float GetResultPowerUPDropRate() const { return ResultPowerUPDropRate; }
public:
    UFUNCTION(BlueprintCallable)
    void UpdateCharacterStatus();

    UFUNCTION(BlueprintCallable, Category = "Status")
    void InitializeStatsFromDataTable(const FDataTableRowHandle& InDataHandle);

	UFUNCTION(BlueprintCallable, Exec, Category = "Debug")
	void Debug_TestAllStats();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

   

private:

public:

protected:
	// =================================================================
	// [상태 변수 (Dynamic State)] - 변동되는 값 (분리 대상 아님)
	// =================================================================
	
	//현재체력
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|State")
	float CurrentHP = 0.0f;

	//현재골드
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|State")
	float CurrentGold = 0.0f;

	//현재 EXP양
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|State")
	float CurrentExp = 0.0f;

	//현재 LV
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Growth")
	int32 CurrentLevel = 1;

	//경험치 통 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Growth")
	float MaxExp = 60.0f; // 1레벨 필요 경험치 (경험치 통)
	
	// 레벨업 시 필요 경험치 증가 배율 (예: 20% 증가)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Growth")
	float ExpGrowthRate = 1.2f; 


	// =================================================================
	// [생존 (Survival)]
	// =================================================================

	// 최대 체력 (MaxHP)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float PlayerMaxHP = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float VisionMaxHP = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float ShrineMaxHP = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultMaxHP = 1.0f;

	// 체력 재생 (HPRegen)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float PlayerHPRegen = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float VisionHPRegen = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float ShrineHPRegen = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultHPRegen = 0.0f;

	// 오버힐 (OverHeal)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float PlayerOverHeal = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float VisionOverHeal = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float ShrineOverHeal = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultOverHeal = 0.0f;

	// 실드 (Shield)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float PlayerShield = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float VisionShield = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float ShrineShield = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultShield = 0.0f;

	// 방어력 (Armor)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float PlayerArmor = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float VisionArmor = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float ShrineArmor = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultArmor = 0.0f;

	// 회피율 (EvasionChance)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float PlayerEvasionChance = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float VisionEvasionChance = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float ShrineEvasionChance = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultEvasionChance = 0.0f;

	// 생명 흡수 (LifeDrain)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float PlayerLifeDrain = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float VisionLifeDrain = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float ShrineLifeDrain = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultLifeDrain = 0.0f;

	// 가시 (Thorn)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float PlayerThorn = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float VisionThorn = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival")
	float ShrineThorn = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultThorn = 0.0f;


	// =================================================================
	// [공격 (Offense)]
	// =================================================================

	// 데미지 배율 (Damage)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float PlayerDamage = 1.0f; // 기본 1.0 (100%)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float VisionDamage = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float ShrineDamage = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultDamage = 1.0f;

	// 치명타 확률 (CriticalChance)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float PlayerCriticalChance = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float VisionCriticalChance = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float ShrineCriticalChance = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultCriticalChance = 0.0f;

	// 치명타 데미지 (CritDmgRate) - *기존 유지 및 Vision 추가*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float PlayerCritDmgRate = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float VisionCritDmgRate = 0.0f; // [추가]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float ShrineCritDmgRate = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultCritDmgRate = 0.0f;

	// 공격 속도 (AttackSpeed)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float PlayerAttackSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float VisionAttackSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float ShrineAttackSpeed = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultAttackSpeed = 0.0f;

	// 엘리트 데미지 (EliteDamage)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float PlayerEliteDamage = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float VisionEliteDamage = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float ShrineEliteDamage = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultEliteDamage = 0.0f;

	// 넉백 (KnockBack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float PlayerKnockBack = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float VisionKnockBack = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense")
	float ShrineKnockBack = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultKnockBack = 0.0f;


	// =================================================================
	// [발사체 (Projectile)]
	// =================================================================

	// 발사체 수 (ProjectileCount)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile")
	float PlayerProjectileCount = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile")
	float VisionProjectileCount = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile")
	float ShrineProjectileCount = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultProjectileCount = 0.0f;

	// 발사체 반사 횟수 (ProjectileReflectCount)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile")
	float PlayerProjectileReflectCount = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile")
	float VisionProjectileReflectCount = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile")
	float ShrineProjectileReflectCount = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultProjectileReflectCount = 0.0f;

	// 공격 크기 (AttackSize) - *기존 유지*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile")
	float PlayerAttackSize = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile")
	float VisionAttackSize = 0.0f; // 기본값 0 (추가분)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile")
	float ShrineAttackSize = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultAttackSize = 0.0f; // [추가] 결과 변수

	// 발사체 속도 (ProjectileSpeed) - *기존 유지*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile")
	float PlayerProjectileSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile")
	float VisionProjectileSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile")
	float ShrineProjectileSpeed = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultProjectileSpeed = 0.0f;

	// 지속 시간 (AttackDuration)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile")
	float PlayerAttackDuration = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile")
	float VisionAttackDuration = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile")
	float ShrineAttackDuration = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultAttackDuration = 0.0f;


	// =================================================================
	// [이동 (Movement)]
	// =================================================================

	// 이동 속도 (MoveSpeed)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Movement")
	float PlayerMoveSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Movement")
	float VisionMoveSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Movement")
	float ShrineMoveSpeed = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultMoveSpeed = 0.0f;

	// 추가 점프 횟수 (ExtraJump)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Movement")
	float PlayerExtraJump = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Movement")
	float VisionExtraJump = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Movement")
	float ShrineExtraJump = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultExtraJump = 0.0f;

	// 점프 높이 (JumpPower)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Movement")
	float PlayerJumpPower = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Movement")
	float VisionJumpPower = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Movement")
	float ShrineJumpPower = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultJumpPower = 0.0f;

	// =================================================================
	// [유틸리티 (Utility)]
	// =================================================================

	// 행운 (Luck)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float PlayerLuck = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float VisionLuck = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float ShrineLuck = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultLuck = 0.0f;

	// 난이도 (Difficulty)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float PlayerDifficulty = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float VisionDifficulty = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float ShrineDifficulty = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultDifficulty = 0.0f;

	// 픽업 범위 (PickUpRange)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float PlayerPickUpRange = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float VisionPickUpRange = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float ShrinePickUpRange = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultPickUpRange = 0.0f;

	// 경험치 획득량 (ExpGain)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float PlayerExpGain = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float VisionExpGain = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float ShrineExpGain = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultExpGain = 0.0f;

	// 골드 획득량 (GoldGain)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float PlayerGoldGain = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float VisionGoldGain = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float ShrineGoldGain = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultGoldGain = 0.0f;

	// 실버 획득량 (SilverGain)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float PlayerSilverGain = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float VisionSilverGain = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float ShrineSilverGain = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultSilverGain = 0.0f;

	// 파워업 배율 (PowerUPRate)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float PlayerPowerUPRate = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float VisionPowerUPRate = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float ShrinePowerUPRate = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultPowerUPRate = 0.0f;

	// 파워업 드롭 확률 (PowerUPDropRate)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float PlayerPowerUPDropRate = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float VisionPowerUPDropRate = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility")
	float ShrinePowerUPDropRate = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Result")
	float ResultPowerUPDropRate = 0.0f;

private:

    TObjectPtr<ACharacter> OwnerCharacter = nullptr;

    TObjectPtr<class UCharacterMovementComponent> Movement = nullptr;

	
};
