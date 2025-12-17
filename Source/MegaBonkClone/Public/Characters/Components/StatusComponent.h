// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

struct FCharacterData;
class UResourceBarWidget; //HP,Shield바

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEGABONKCLONE_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

public:
	// =================================================================
	// [상태 변수 (State)] - 분리 대상 아님 (기존 유지)
	// =================================================================
	UFUNCTION(BlueprintCallable)
	inline float GetCurrentHP() const { return CurrentHP; }
	UFUNCTION(BlueprintCallable)
	inline void SetCurrentHP(float NewCurrentHP) { CurrentHP = NewCurrentHP; }

	UFUNCTION(BlueprintCallable)
	inline float GetCurrentGold() const { return CurrentGold; }
	UFUNCTION(BlueprintCallable)
	inline void SetCurrentGold(float NewCurrentGold) { CurrentGold = NewCurrentGold; }

	UFUNCTION(BlueprintCallable)
	inline float GetCurrentExp() const { return CurrentExp; }
	UFUNCTION(BlueprintCallable)
	inline void SetCurrentExp(float NewCurrentExp) { CurrentExp = NewCurrentExp; }


	// =================================================================
	// [생존 (Survival)]
	// =================================================================

	// MaxHP
	UFUNCTION(BlueprintCallable) inline float GetPlayerMaxHP() const { return PlayerMaxHP; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerMaxHP(float Val) { PlayerMaxHP = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionMaxHP() const { return VisionMaxHP; }
	UFUNCTION(BlueprintCallable) inline void SetVisionMaxHP(float Val) { VisionMaxHP = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineMaxHP() const { return ShrineMaxHP; }
	UFUNCTION(BlueprintCallable) inline void SetShrineMaxHP(float Val) { ShrineMaxHP = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultMaxHP() const { return ResultMaxHP; }
	UFUNCTION(BlueprintCallable) inline void SetResultMaxHP(float Val) { ResultMaxHP = Val; }

	// HPRegen
	UFUNCTION(BlueprintCallable) inline float GetPlayerHPRegen() const { return PlayerHPRegen; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerHPRegen(float Val) { PlayerHPRegen = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionHPRegen() const { return VisionHPRegen; }
	UFUNCTION(BlueprintCallable) inline void SetVisionHPRegen(float Val) { VisionHPRegen = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineHPRegen() const { return ShrineHPRegen; }
	UFUNCTION(BlueprintCallable) inline void SetShrineHPRegen(float Val) { ShrineHPRegen = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultHPRegen() const { return ResultHPRegen; }
	UFUNCTION(BlueprintCallable) inline void SetResultHPRegen(float Val) { ResultHPRegen = Val; }

	// OverHeal
	UFUNCTION(BlueprintCallable) inline float GetPlayerOverHeal() const { return PlayerOverHeal; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerOverHeal(float Val) { PlayerOverHeal = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionOverHeal() const { return VisionOverHeal; }
	UFUNCTION(BlueprintCallable) inline void SetVisionOverHeal(float Val) { VisionOverHeal = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineOverHeal() const { return ShrineOverHeal; }
	UFUNCTION(BlueprintCallable) inline void SetShrineOverHeal(float Val) { ShrineOverHeal = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultOverHeal() const { return ResultOverHeal; }
	UFUNCTION(BlueprintCallable) inline void SetResultOverHeal(float Val) { ResultOverHeal = Val; }

	// Shield
	UFUNCTION(BlueprintCallable) inline float GetPlayerShield() const { return PlayerShield; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerShield(float Val) { PlayerShield = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionShield() const { return VisionShield; }
	UFUNCTION(BlueprintCallable) inline void SetVisionShield(float Val) { VisionShield = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineShield() const { return ShrineShield; }
	UFUNCTION(BlueprintCallable) inline void SetShrineShield(float Val) { ShrineShield = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultShield() const { return ResultShield; }
	UFUNCTION(BlueprintCallable) inline void SetResultShield(float Val) { ResultShield = Val; }

	// Armor
	UFUNCTION(BlueprintCallable) inline float GetPlayerArmor() const { return PlayerArmor; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerArmor(float Val) { PlayerArmor = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionArmor() const { return VisionArmor; }
	UFUNCTION(BlueprintCallable) inline void SetVisionArmor(float Val) { VisionArmor = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineArmor() const { return ShrineArmor; }
	UFUNCTION(BlueprintCallable) inline void SetShrineArmor(float Val) { ShrineArmor = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultArmor() const { return ResultArmor; }
	UFUNCTION(BlueprintCallable) inline void SetResultArmor(float Val) { ResultArmor = Val; }

	// EvasionChance
	UFUNCTION(BlueprintCallable) inline float GetPlayerEvasionChance() const { return PlayerEvasionChance; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerEvasionChance(float Val) { PlayerEvasionChance = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionEvasionChance() const { return VisionEvasionChance; }
	UFUNCTION(BlueprintCallable) inline void SetVisionEvasionChance(float Val) { VisionEvasionChance = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineEvasionChance() const { return ShrineEvasionChance; }
	UFUNCTION(BlueprintCallable) inline void SetShrineEvasionChance(float Val) { ShrineEvasionChance = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultEvasionChance() const { return ResultEvasionChance; }
	UFUNCTION(BlueprintCallable) inline void SetResultEvasionChance(float Val) { ResultEvasionChance = Val; }

	// LifeDrain
	UFUNCTION(BlueprintCallable) inline float GetPlayerLifeDrain() const { return PlayerLifeDrain; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerLifeDrain(float Val) { PlayerLifeDrain = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionLifeDrain() const { return VisionLifeDrain; }
	UFUNCTION(BlueprintCallable) inline void SetVisionLifeDrain(float Val) { VisionLifeDrain = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineLifeDrain() const { return ShrineLifeDrain; }
	UFUNCTION(BlueprintCallable) inline void SetShrineLifeDrain(float Val) { ShrineLifeDrain = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultLifeDrain() const { return ResultLifeDrain; }
	UFUNCTION(BlueprintCallable) inline void SetResultLifeDrain(float Val) { ResultLifeDrain = Val; }

	// Thorn
	UFUNCTION(BlueprintCallable) inline float GetPlayerThorn() const { return PlayerThorn; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerThorn(float Val) { PlayerThorn = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionThorn() const { return VisionThorn; }
	UFUNCTION(BlueprintCallable) inline void SetVisionThorn(float Val) { VisionThorn = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineThorn() const { return ShrineThorn; }
	UFUNCTION(BlueprintCallable) inline void SetShrineThorn(float Val) { ShrineThorn = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultThorn() const { return ResultThorn; }
	UFUNCTION(BlueprintCallable) inline void SetResultThorn(float Val) { ResultThorn = Val; }


	// =================================================================
	// [공격 (Offense)]
	// =================================================================

	// Damage
	UFUNCTION(BlueprintCallable) inline float GetPlayerDamage() const { return PlayerDamage; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerDamage(float Val) { PlayerDamage = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionDamage() const { return VisionDamage; }
	UFUNCTION(BlueprintCallable) inline void SetVisionDamage(float Val) { VisionDamage = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineDamage() const { return ShrineDamage; }
	UFUNCTION(BlueprintCallable) inline void SetShrineDamage(float Val) { ShrineDamage = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultDamage() const { return ResultDamage; }
	UFUNCTION(BlueprintCallable) inline void SetResultDamage(float Val) { ResultDamage = Val; }

	// CriticalChance
	UFUNCTION(BlueprintCallable) inline float GetPlayerCriticalChance() const { return PlayerCriticalChance; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerCriticalChance(float Val) { PlayerCriticalChance = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionCriticalChance() const { return VisionCriticalChance; }
	UFUNCTION(BlueprintCallable) inline void SetVisionCriticalChance(float Val) { VisionCriticalChance = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineCriticalChance() const { return ShrineCriticalChance; }
	UFUNCTION(BlueprintCallable) inline void SetShrineCriticalChance(float Val) { ShrineCriticalChance = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultCriticalChance() const { return ResultCriticalChance; }
	UFUNCTION(BlueprintCallable) inline void SetResultCriticalChance(float Val) { ResultCriticalChance = Val; }

	// CritDmgRate
	UFUNCTION(BlueprintCallable) inline float GetPlayerCritDmgRate() const { return PlayerCritDmgRate; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerCritDmgRate(float Val) { PlayerCritDmgRate = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionCritDmgRate() const { return VisionCritDmgRate; }
	UFUNCTION(BlueprintCallable) inline void SetVisionCritDmgRate(float Val) { VisionCritDmgRate = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineCritDmgRate() const { return ShrineCritDmgRate; }
	UFUNCTION(BlueprintCallable) inline void SetShrineCritDmgRate(float Val) { ShrineCritDmgRate = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultCritDmgRate() const { return ResultCritDmgRate; }
	UFUNCTION(BlueprintCallable) inline void SetResultCritDmgRate(float Val) { ResultCritDmgRate = Val; }

	// AttackSpeed
	UFUNCTION(BlueprintCallable) inline float GetPlayerAttackSpeed() const { return PlayerAttackSpeed; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerAttackSpeed(float Val) { PlayerAttackSpeed = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionAttackSpeed() const { return VisionAttackSpeed; }
	UFUNCTION(BlueprintCallable) inline void SetVisionAttackSpeed(float Val) { VisionAttackSpeed = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineAttackSpeed() const { return ShrineAttackSpeed; }
	UFUNCTION(BlueprintCallable) inline void SetShrineAttackSpeed(float Val) { ShrineAttackSpeed = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultAttackSpeed() const { return ResultAttackSpeed; }
	UFUNCTION(BlueprintCallable) inline void SetResultAttackSpeed(float Val) { ResultAttackSpeed = Val; }

	// EliteDamage
	UFUNCTION(BlueprintCallable) inline float GetPlayerEliteDamage() const { return PlayerEliteDamage; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerEliteDamage(float Val) { PlayerEliteDamage = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionEliteDamage() const { return VisionEliteDamage; }
	UFUNCTION(BlueprintCallable) inline void SetVisionEliteDamage(float Val) { VisionEliteDamage = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineEliteDamage() const { return ShrineEliteDamage; }
	UFUNCTION(BlueprintCallable) inline void SetShrineEliteDamage(float Val) { ShrineEliteDamage = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultEliteDamage() const { return ResultEliteDamage; }
	UFUNCTION(BlueprintCallable) inline void SetResultEliteDamage(float Val) { ResultEliteDamage = Val; }

	// KnockBack
	UFUNCTION(BlueprintCallable) inline float GetPlayerKnockBack() const { return PlayerKnockBack; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerKnockBack(float Val) { PlayerKnockBack = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionKnockBack() const { return VisionKnockBack; }
	UFUNCTION(BlueprintCallable) inline void SetVisionKnockBack(float Val) { VisionKnockBack = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineKnockBack() const { return ShrineKnockBack; }
	UFUNCTION(BlueprintCallable) inline void SetShrineKnockBack(float Val) { ShrineKnockBack = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultKnockBack() const { return ResultKnockBack; }
	UFUNCTION(BlueprintCallable) inline void SetResultKnockBack(float Val) { ResultKnockBack = Val; }


	// =================================================================
	// [발사체 (Projectile)]
	// =================================================================

	// ProjectileCount
	UFUNCTION(BlueprintCallable) inline float GetPlayerProjectileCount() const { return PlayerProjectileCount; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerProjectileCount(float Val) { PlayerProjectileCount = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionProjectileCount() const { return VisionProjectileCount; }
	UFUNCTION(BlueprintCallable) inline void SetVisionProjectileCount(float Val) { VisionProjectileCount = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineProjectileCount() const { return ShrineProjectileCount; }
	UFUNCTION(BlueprintCallable) inline void SetShrineProjectileCount(float Val) { ShrineProjectileCount = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultProjectileCount() const { return ResultProjectileCount; }
	UFUNCTION(BlueprintCallable) inline void SetResultProjectileCount(float Val) { ResultProjectileCount = Val; }

	// ProjectileReflectCount
	UFUNCTION(BlueprintCallable) inline float GetPlayerProjectileReflectCount() const { return PlayerProjectileReflectCount; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerProjectileReflectCount(float Val) { PlayerProjectileReflectCount = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionProjectileReflectCount() const { return VisionProjectileReflectCount; }
	UFUNCTION(BlueprintCallable) inline void SetVisionProjectileReflectCount(float Val) { VisionProjectileReflectCount = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineProjectileReflectCount() const { return ShrineProjectileReflectCount; }
	UFUNCTION(BlueprintCallable) inline void SetShrineProjectileReflectCount(float Val) { ShrineProjectileReflectCount = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultProjectileReflectCount() const { return ResultProjectileReflectCount; }
	UFUNCTION(BlueprintCallable) inline void SetResultProjectileReflectCount(float Val) { ResultProjectileReflectCount = Val; }

	// AttackSize
	UFUNCTION(BlueprintCallable) inline float GetPlayerAttackSize() const { return PlayerAttackSize; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerAttackSize(float Val) { PlayerAttackSize = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionAttackSize() const { return VisionAttackSize; }
	UFUNCTION(BlueprintCallable) inline void SetVisionAttackSize(float Val) { VisionAttackSize = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineAttackSize() const { return ShrineAttackSize; }
	UFUNCTION(BlueprintCallable) inline void SetShrineAttackSize(float Val) { ShrineAttackSize = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultAttackSize() const { return ResultAttackSize; }
	UFUNCTION(BlueprintCallable) inline void SetResultAttackSize(float Val) { ResultAttackSize = Val; }

	// ProjectileSpeed
	UFUNCTION(BlueprintCallable) inline float GetPlayerProjectileSpeed() const { return PlayerProjectileSpeed; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerProjectileSpeed(float Val) { PlayerProjectileSpeed = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionProjectileSpeed() const { return VisionProjectileSpeed; }
	UFUNCTION(BlueprintCallable) inline void SetVisionProjectileSpeed(float Val) { VisionProjectileSpeed = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineProjectileSpeed() const { return ShrineProjectileSpeed; }
	UFUNCTION(BlueprintCallable) inline void SetShrineProjectileSpeed(float Val) { ShrineProjectileSpeed = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultProjectileSpeed() const { return ResultProjectileSpeed; }
	UFUNCTION(BlueprintCallable) inline void SetResultProjectileSpeed(float Val) { ResultProjectileSpeed = Val; }

	// AttackDuration
	UFUNCTION(BlueprintCallable) inline float GetPlayerAttackDuration() const { return PlayerAttackDuration; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerAttackDuration(float Val) { PlayerAttackDuration = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionAttackDuration() const { return VisionAttackDuration; }
	UFUNCTION(BlueprintCallable) inline void SetVisionAttackDuration(float Val) { VisionAttackDuration = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineAttackDuration() const { return ShrineAttackDuration; }
	UFUNCTION(BlueprintCallable) inline void SetShrineAttackDuration(float Val) { ShrineAttackDuration = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultAttackDuration() const { return ResultAttackDuration; }
	UFUNCTION(BlueprintCallable) inline void SetResultAttackDuration(float Val) { ResultAttackDuration = Val; }


	// =================================================================
	// [이동 (Movement)]
	// =================================================================

	// MoveSpeed
	UFUNCTION(BlueprintCallable) inline float GetPlayerMoveSpeed() const { return PlayerMoveSpeed; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerMoveSpeed(float Val) { PlayerMoveSpeed = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionMoveSpeed() const { return VisionMoveSpeed; }
	UFUNCTION(BlueprintCallable) inline void SetVisionMoveSpeed(float Val) { VisionMoveSpeed = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineMoveSpeed() const { return ShrineMoveSpeed; }
	UFUNCTION(BlueprintCallable) inline void SetShrineMoveSpeed(float Val) { ShrineMoveSpeed = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultMoveSpeed() const { return ResultMoveSpeed; }
	UFUNCTION(BlueprintCallable) inline void SetResultMoveSpeed(float Val) { ResultMoveSpeed = Val; }

	// ExtraJump
	UFUNCTION(BlueprintCallable) inline float GetPlayerExtraJump() const { return PlayerExtraJump; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerExtraJump(float Val) { PlayerExtraJump = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionExtraJump() const { return VisionExtraJump; }
	UFUNCTION(BlueprintCallable) inline void SetVisionExtraJump(float Val) { VisionExtraJump = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineExtraJump() const { return ShrineExtraJump; }
	UFUNCTION(BlueprintCallable) inline void SetShrineExtraJump(float Val) { ShrineExtraJump = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultExtraJump() const { return ResultExtraJump; }
	UFUNCTION(BlueprintCallable) inline void SetResultExtraJump(float Val) { ResultExtraJump = Val; }

	// JumpPower
	UFUNCTION(BlueprintCallable) inline float GetPlayerJumpPower() const { return PlayerJumpPower; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerJumpPower(float Val) { PlayerJumpPower = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionJumpPower() const { return VisionJumpPower; }
	UFUNCTION(BlueprintCallable) inline void SetVisionJumpPower(float Val) { VisionJumpPower = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineJumpPower() const { return ShrineJumpPower; }
	UFUNCTION(BlueprintCallable) inline void SetShrineJumpPower(float Val) { ShrineJumpPower = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultJumpPower() const { return ResultJumpPower; }
	UFUNCTION(BlueprintCallable) inline void SetResultJumpPower(float Val) { ResultJumpPower = Val; }


	// =================================================================
	// [유틸리티 (Utility)]
	// =================================================================

	// Luck
	UFUNCTION(BlueprintCallable) inline float GetPlayerLuck() const { return PlayerLuck; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerLuck(float Val) { PlayerLuck = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionLuck() const { return VisionLuck; }
	UFUNCTION(BlueprintCallable) inline void SetVisionLuck(float Val) { VisionLuck = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineLuck() const { return ShrineLuck; }
	UFUNCTION(BlueprintCallable) inline void SetShrineLuck(float Val) { ShrineLuck = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultLuck() const { return ResultLuck; }
	UFUNCTION(BlueprintCallable) inline void SetResultLuck(float Val) { ResultLuck = Val; }

	// Difficulty
	UFUNCTION(BlueprintCallable) inline float GetPlayerDifficulty() const { return PlayerDifficulty; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerDifficulty(float Val) { PlayerDifficulty = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionDifficulty() const { return VisionDifficulty; }
	UFUNCTION(BlueprintCallable) inline void SetVisionDifficulty(float Val) { VisionDifficulty = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineDifficulty() const { return ShrineDifficulty; }
	UFUNCTION(BlueprintCallable) inline void SetShrineDifficulty(float Val) { ShrineDifficulty = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultDifficulty() const { return ResultDifficulty; }
	UFUNCTION(BlueprintCallable) inline void SetResultDifficulty(float Val) { ResultDifficulty = Val; }

	// PickUpRange
	UFUNCTION(BlueprintCallable) inline float GetPlayerPickUpRange() const { return PlayerPickUpRange; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerPickUpRange(float Val) { PlayerPickUpRange = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionPickUpRange() const { return VisionPickUpRange; }
	UFUNCTION(BlueprintCallable) inline void SetVisionPickUpRange(float Val) { VisionPickUpRange = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrinePickUpRange() const { return ShrinePickUpRange; }
	UFUNCTION(BlueprintCallable) inline void SetShrinePickUpRange(float Val) { ShrinePickUpRange = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultPickUpRange() const { return ResultPickUpRange; }
	UFUNCTION(BlueprintCallable) inline void SetResultPickUpRange(float Val) { ResultPickUpRange = Val; }

	// ExpGain
	UFUNCTION(BlueprintCallable) inline float GetPlayerExpGain() const { return PlayerExpGain; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerExpGain(float Val) { PlayerExpGain = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionExpGain() const { return VisionExpGain; }
	UFUNCTION(BlueprintCallable) inline void SetVisionExpGain(float Val) { VisionExpGain = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineExpGain() const { return ShrineExpGain; }
	UFUNCTION(BlueprintCallable) inline void SetShrineExpGain(float Val) { ShrineExpGain = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultExpGain() const { return ResultExpGain; }
	UFUNCTION(BlueprintCallable) inline void SetResultExpGain(float Val) { ResultExpGain = Val; }

	// GoldGain
	UFUNCTION(BlueprintCallable) inline float GetPlayerGoldGain() const { return PlayerGoldGain; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerGoldGain(float Val) { PlayerGoldGain = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionGoldGain() const { return VisionGoldGain; }
	UFUNCTION(BlueprintCallable) inline void SetVisionGoldGain(float Val) { VisionGoldGain = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineGoldGain() const { return ShrineGoldGain; }
	UFUNCTION(BlueprintCallable) inline void SetShrineGoldGain(float Val) { ShrineGoldGain = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultGoldGain() const { return ResultGoldGain; }
	UFUNCTION(BlueprintCallable) inline void SetResultGoldGain(float Val) { ResultGoldGain = Val; }

	// SilverGain
	UFUNCTION(BlueprintCallable) inline float GetPlayerSilverGain() const { return PlayerSilverGain; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerSilverGain(float Val) { PlayerSilverGain = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionSilverGain() const { return VisionSilverGain; }
	UFUNCTION(BlueprintCallable) inline void SetVisionSilverGain(float Val) { VisionSilverGain = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrineSilverGain() const { return ShrineSilverGain; }
	UFUNCTION(BlueprintCallable) inline void SetShrineSilverGain(float Val) { ShrineSilverGain = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultSilverGain() const { return ResultSilverGain; }
	UFUNCTION(BlueprintCallable) inline void SetResultSilverGain(float Val) { ResultSilverGain = Val; }

	// PowerUPRate
	UFUNCTION(BlueprintCallable) inline float GetPlayerPowerUPRate() const { return PlayerPowerUPRate; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerPowerUPRate(float Val) { PlayerPowerUPRate = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionPowerUPRate() const { return VisionPowerUPRate; }
	UFUNCTION(BlueprintCallable) inline void SetVisionPowerUPRate(float Val) { VisionPowerUPRate = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrinePowerUPRate() const { return ShrinePowerUPRate; }
	UFUNCTION(BlueprintCallable) inline void SetShrinePowerUPRate(float Val) { ShrinePowerUPRate = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultPowerUPRate() const { return ResultPowerUPRate; }
	UFUNCTION(BlueprintCallable) inline void SetResultPowerUPRate(float Val) { ResultPowerUPRate = Val; }

	// PowerUPDropRate
	UFUNCTION(BlueprintCallable) inline float GetPlayerPowerUPDropRate() const { return PlayerPowerUPDropRate; }
	UFUNCTION(BlueprintCallable) inline void SetPlayerPowerUPDropRate(float Val) { PlayerPowerUPDropRate = Val; }
	UFUNCTION(BlueprintCallable) inline float GetVisionPowerUPDropRate() const { return VisionPowerUPDropRate; }
	UFUNCTION(BlueprintCallable) inline void SetVisionPowerUPDropRate(float Val) { VisionPowerUPDropRate = Val; }
	UFUNCTION(BlueprintCallable) inline float GetShrinePowerUPDropRate() const { return ShrinePowerUPDropRate; }
	UFUNCTION(BlueprintCallable) inline void SetShrinePowerUPDropRate(float Val) { ShrinePowerUPDropRate = Val; }
	UFUNCTION(BlueprintCallable) inline float GetResultPowerUPDropRate() const { return ResultPowerUPDropRate; }
	UFUNCTION(BlueprintCallable) inline void SetResultPowerUPDropRate(float Val) { ResultPowerUPDropRate = Val; }


public:
    UFUNCTION(BlueprintCallable)
    void UpdateCharacterStatus();

    UFUNCTION(BlueprintCallable)
    float GetStatusDamage();

    UFUNCTION(BlueprintCallable, Category = "Status")
    void InitializeStatsFromDataTable(const FDataTableRowHandle& InDataHandle);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

   

private:

public:

protected:
	// =================================================================
	// [상태 변수 (Dynamic State)] - 변동되는 값 (분리 대상 아님)
	// =================================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|State")
	float CurrentHP = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|State")
	float CurrentGold = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|State")
	float CurrentExp = 0.0f;


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
	float ResultAttackSpeed = 1.0f;

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
