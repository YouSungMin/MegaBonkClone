// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEGABONKCLONE_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

public:
    //생각 중인 구현방법 스탯을 건드리는 아이템을 먹거나 스탯이 변화하면 현재 캐릭터 스탯을 변경하고 
    //UpdateCharacterStatus() 통해 이용가능한 실제값으로 변환후 적용
    // =================================================================
    // 생명력 및 방어 관련 (Health & Defense)
    // =================================================================

    // MaxHP
    UFUNCTION(BlueprintCallable)
    inline float GetMaxHP() const { return MaxHP; }
    UFUNCTION(BlueprintCallable)
    inline void SetMaxHP(float NewMaxHP) { MaxHP = NewMaxHP; }

    // HPRegen
    UFUNCTION(BlueprintCallable)
    inline float GetHPRegen() const { return HPRegen; }
    UFUNCTION(BlueprintCallable)
    inline void SetHPRegen(float NewHPRegen) { HPRegen = NewHPRegen; }

    // OverHeal
    UFUNCTION(BlueprintCallable)
    inline float GetOverHeal() const { return OverHeal; }
    UFUNCTION(BlueprintCallable)
    inline void SetOverHeal(float NewOverHeal) { OverHeal = NewOverHeal; }

    // Shield
    UFUNCTION(BlueprintCallable)
    inline float GetShield() const { return Shield; }
    UFUNCTION(BlueprintCallable)
    inline void SetShield(float NewShield) { Shield = NewShield; }

    // Armor
    UFUNCTION(BlueprintCallable)
    inline float GetArmor() const { return Armor; }
    UFUNCTION(BlueprintCallable)
    inline void SetArmor(float NewArmor) { Armor = NewArmor; }

    // EvasionChance
    UFUNCTION(BlueprintCallable)
    inline float GetEvasionChance() const { return EvasionChance; }
    UFUNCTION(BlueprintCallable)
    inline void SetEvasionChance(float NewEvasionChance) { EvasionChance = NewEvasionChance; }

    // LifeDrain
    UFUNCTION(BlueprintCallable)
    inline float GetLifeDrain() const { return LifeDrain; }
    UFUNCTION(BlueprintCallable)
    inline void SetLifeDrain(float NewLifeDrain) { LifeDrain = NewLifeDrain; }

    // Thorn
    UFUNCTION(BlueprintCallable)
    inline float GetThorn() const { return Thorn; }
    UFUNCTION(BlueprintCallable)
    inline void SetThorn(float NewThorn) { Thorn = NewThorn; }


    // =================================================================
    // 재화 및 성장 관련 (Resources & Growth)
    // =================================================================

    // ExpGain
    UFUNCTION(BlueprintCallable)
    inline float GetExpGain() const { return ExpGain; }
    UFUNCTION(BlueprintCallable)
    inline void SetExpGain(float NewExpGain) { ExpGain = NewExpGain; }

    // SilverGain
    UFUNCTION(BlueprintCallable)
    inline float GetSilverGain() const { return SilverGain; }
    UFUNCTION(BlueprintCallable)
    inline void SetSilverGain(float NewSilverGain) { SilverGain = NewSilverGain; }

    // GoldGain
    UFUNCTION(BlueprintCallable)
    inline float GetGoldGain() const { return GoldGain; }
    UFUNCTION(BlueprintCallable)
    inline void SetGoldGain(float NewGoldGain) { GoldGain = NewGoldGain; }

    // PowerUPRate
    UFUNCTION(BlueprintCallable)
    inline float GetPowerUPRate() const { return PowerUPRate; }
    UFUNCTION(BlueprintCallable)
    inline void SetPowerUPRate(float NewPowerUPRate) { PowerUPRate = NewPowerUPRate; }

    // PowerUPDropRate
    UFUNCTION(BlueprintCallable)
    inline float GetPowerUPDropRate() const { return PowerUPDropRate; }
    UFUNCTION(BlueprintCallable)
    inline void SetPowerUPDropRate(float NewPowerUPDropRate) { PowerUPDropRate = NewPowerUPDropRate; }

    // PickUpRange
    UFUNCTION(BlueprintCallable)
    inline float GetPickUpRange() const { return PickUpRange; }
    UFUNCTION(BlueprintCallable)
    inline void SetPickUpRange(float NewPickUpRange) { PickUpRange = NewPickUpRange; }

    // Difficulty
    UFUNCTION(BlueprintCallable)
    inline float GetDifficulty() const { return Difficulty; }
    UFUNCTION(BlueprintCallable)
    inline void SetDifficulty(float NewDifficulty) { Difficulty = NewDifficulty; }

    // Luck
    UFUNCTION(BlueprintCallable)
    inline float GetLuck() const { return Luck; }
    UFUNCTION(BlueprintCallable)
    inline void SetLuck(float NewLuck) { Luck = NewLuck; }


    // =================================================================
    // 공격 및 전투 관련 (Attack & Combat)
    // =================================================================

    // Damage
    UFUNCTION(BlueprintCallable)
    inline float GetDamage() const { return Damage; }
    UFUNCTION(BlueprintCallable)
    inline void SetDamage(float NewDamage) { Damage = NewDamage; }

    // CriticalChance
    UFUNCTION(BlueprintCallable)
    inline float GetCriticalChance() const { return CriticalChance; }
    UFUNCTION(BlueprintCallable)
    inline void SetCriticalChance(float NewCriticalChance) { CriticalChance = NewCriticalChance; }

    // PlayerCritDmgRate
    UFUNCTION(BlueprintCallable)
    inline float GetPlayerCritDmgRate() const { return PlayerCritDmgRate; }
    UFUNCTION(BlueprintCallable)
    inline void SetPlayerCritDmgRate(float NewPlayerCritDmgRate) { PlayerCritDmgRate = NewPlayerCritDmgRate; }

    // ShrineCritDmgRate
    UFUNCTION(BlueprintCallable)
    inline float GetShrineCritDmgRate() const { return ShrineCritDmgRate; }
    UFUNCTION(BlueprintCallable)
    inline void SetShrineCritDmgRate(float NewShrineCritDmgRate) { ShrineCritDmgRate = NewShrineCritDmgRate; }

    // ResultCritDmgRate
    UFUNCTION(BlueprintCallable)
    inline float GetResultCritDmgRate() const { return ResultCritDmgRate; }
    UFUNCTION(BlueprintCallable)
    inline void SetResultCritDmgRate(float NewResultCritDmgRate) { ResultCritDmgRate = NewResultCritDmgRate; }

    // AttackSpeed
    UFUNCTION(BlueprintCallable)
    inline float GetAttackSpeed() const { return AttackSpeed; }
    UFUNCTION(BlueprintCallable)
    inline void SetAttackSpeed(float NewAttackSpeed) { AttackSpeed = NewAttackSpeed; }

    // AttackSize
    UFUNCTION(BlueprintCallable)
    inline float GetAttackSize() const { return AttackSize; }
    UFUNCTION(BlueprintCallable)
    inline void SetAttackSize(float NewAttackSize) { AttackSize = NewAttackSize; }

    // AttackDuration
    UFUNCTION(BlueprintCallable)
    inline float GetAttackDuration() const { return AttackDuration; }
    UFUNCTION(BlueprintCallable)
    inline void SetAttackDuration(float NewAttackDuration) { AttackDuration = NewAttackDuration; }

    // EliteDamage
    UFUNCTION(BlueprintCallable)
    inline float GetEliteDamage() const { return EliteDamage; }
    UFUNCTION(BlueprintCallable)
    inline void SetEliteDamage(float NewEliteDamage) { EliteDamage = NewEliteDamage; }

    // KnockBack
    UFUNCTION(BlueprintCallable)
    inline float GetKnockBack() const { return KnockBack; }
    UFUNCTION(BlueprintCallable)
    inline void SetKnockBack(float NewKnockBack) { KnockBack = NewKnockBack; }


    // =================================================================
    // 발사체 관련 (Projectile)
    // =================================================================

    // ProjectileCount
    UFUNCTION(BlueprintCallable)
    inline float GetProjectileCount() const { return ProjectileCount; }
    UFUNCTION(BlueprintCallable)
    inline void SetProjectileCount(float NewProjectileCount) { ProjectileCount = NewProjectileCount; }

    // ProjectileReflectCount
    UFUNCTION(BlueprintCallable)
    inline float GetProjectileReflectCount() const { return ProjectileReflectCount; }
    UFUNCTION(BlueprintCallable)
    inline void SetProjectileReflectCount(float NewProjectileReflectCount) { ProjectileReflectCount = NewProjectileReflectCount; }

    // ShrineProjectileSpeed
    UFUNCTION(BlueprintCallable)
    inline float GetShrineProjectileSpeed() const { return ShrineProjectileSpeed; }
    UFUNCTION(BlueprintCallable)
    inline void SetShrineProjectileSpeed(float NewShrineProjectileSpeed) { ShrineProjectileSpeed = NewShrineProjectileSpeed; }

    // VisionProjectileSpeed
    UFUNCTION(BlueprintCallable)
    inline float GetVisionProjectileSpeed() const { return VisionProjectileSpeed; }
    UFUNCTION(BlueprintCallable)
    inline void SetVisionProjectileSpeed(float NewVisionProjectileSpeed) { VisionProjectileSpeed = NewVisionProjectileSpeed; }

    // PlayerProjectileSpeed
    UFUNCTION(BlueprintCallable)
    inline float GetPlayerProjectileSpeed() const { return PlayerProjectileSpeed; }
    UFUNCTION(BlueprintCallable)
    inline void SetPlayerProjectileSpeed(float NewPlayerProjectileSpeed) { PlayerProjectileSpeed = NewPlayerProjectileSpeed; }

    // ResultProjectileSpeed
    UFUNCTION(BlueprintCallable)
    inline float GetResultProjectileSpeed() const { return ResultProjectileSpeed; }
    UFUNCTION(BlueprintCallable)
    inline void SetResultProjectileSpeed(float NewResultProjectileSpeed) { ResultProjectileSpeed = NewResultProjectileSpeed; }


    // =================================================================
    // 이동 관련 (Movement)
    // =================================================================

    // MoveSpeed
    UFUNCTION(BlueprintCallable)
    inline float GetMoveSpeed() const { return MoveSpeed; }
    UFUNCTION(BlueprintCallable)
    inline void SetMoveSpeed(float NewMoveSpeed) { MoveSpeed = NewMoveSpeed; }

    // ExtraJump
    UFUNCTION(BlueprintCallable)
    inline float GetExtraJump() const { return ExtraJump; }
    UFUNCTION(BlueprintCallable)
    inline void SetExtraJump(float NewExtraJump) { ExtraJump = NewExtraJump; }

    // JumpPower
    UFUNCTION(BlueprintCallable)
    inline float GetJumpPower() const { return JumpPower; }
    UFUNCTION(BlueprintCallable)
    inline void SetJumpPower(float NewJumpPower) { JumpPower = NewJumpPower; }



public:
    UFUNCTION(BlueprintCallable)
    void UpdateCharacterStatus();

    UFUNCTION(BlueprintCallable)
    float GetFinalDamage();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

   

private:

public:

protected:
	//최대체력 (수치그대로)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float MaxHP = 0.0f;
	//HP재생 (수치/60) = 초당회복량
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float HPRegen = 0.0f;
	//오버힐 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float OverHeal = 0.0f;
	//실드 (방어막느낌) (수치그대로)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float Shield = 0.0f;
	//아머 (방어력)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float Armor = 0.0f;
	//회피율
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float EvasionChance = 0.0f;
	//생명 흡수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float LifeDrain = 0.0f;
	//가시(사용 안할듯)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float Thorn = 0.0f;
	//경험치 획득량
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float ExpGain = 0.0f;
	//실버 획득량 (사용 안할듯)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float SilverGain = 0.0f;
	//파워업배율(합적용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float PowerUPRate = 0.0f;
	//파워업 드롭 확률
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float PowerUPDropRate = 0.0f;
	//픽업범위
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float PickUpRange = 10.0f;
	//난이도(합적용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float Difficulty = 0.0f;
	//데미지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float Damage = 0.0f;
	//크리티컬확률(수치그대로)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float CriticalChance = 0.0f;
	//크리티컬데미지(플레이어 스탯)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float PlayerCritDmgRate = 0.0f;
	//크리티컬데미지(성소 스탯)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float ShrineCritDmgRate = 0.0f;
	//계산된크리티컬데미지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float ResultCritDmgRate = 0.0f;
	//공격속도 (합연산)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float AttackSpeed = 0.0f;
	//발사체수 (합연산) 소숫점아래버림
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float ProjectileCount = 0.0f;
	//발사체반사횟수 (합연산) 소숫점아래버림
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float ProjectileReflectCount = 0.0f;
	//크기를지원하는 무기의 크기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float AttackSize = 0.0f;
	//발사체 속도 (성소)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float ShrineProjectileSpeed = 0.0f;
	//발사체 속도 (비전서)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float VisionProjectileSpeed = 0.0f;
	//발사체 속도 (플레이어)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float PlayerProjectileSpeed = 0.0f;
	//계산된 발사체 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float ResultProjectileSpeed = 0.0f;
	//지속시간
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float AttackDuration = 0.0f;
	//엘리트데미지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float EliteDamage = 0.0f;
	//넉백
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float KnockBack = 0.0f;
	//이동속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float MoveSpeed = 0.0f;
	//추가점프횟수 (합연산) 소숫점아래버림
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float ExtraJump = 0.0f;
	//점프높이
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float JumpPower = 0.0f;
	//행운 (합연산)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
	float Luck = 0.0f;
    //골드 획득량
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Status")
    float GoldGain = 0.0f;


    
private:

    TObjectPtr<ACharacter> OwnerCharacter = nullptr;

    TObjectPtr<class UCharacterMovementComponent> Movement = nullptr;

	
};
