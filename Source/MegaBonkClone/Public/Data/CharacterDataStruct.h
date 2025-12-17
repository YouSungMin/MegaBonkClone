// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterDataStruct.generated.h"


USTRUCT(BlueprintType)
struct FCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// ============================================================
	// [정보]
	// ============================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (DisplayName = "캐릭터 이름"))
	FText CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (DisplayName = "캐릭터 설명", MultiLine = "true"))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual", meta = (DisplayName = "초상화 아이콘"))
	TSoftObjectPtr<UTexture2D> PortraitIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual", meta = (DisplayName = "메쉬 (Skeletal Mesh)"))
	TSoftObjectPtr<USkeletalMesh> SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual", meta = (DisplayName = "기본 무기 (Base Weapon)"))
	TSoftClassPtr<class AWeaponBase> BaseWeapon = nullptr;

	// ============================================================
	// [생존 (Survival)]
	// ============================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival", meta = (DisplayName = "최대 HP (Max HP)"))
	float MaxHP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival", meta = (DisplayName = "HP 회복 (Regen)"))
	float HPRegen = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival", meta = (DisplayName = "오버힐 (OverHeal)"))
	float OverHeal = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival", meta = (DisplayName = "쉴드 (Shield)"))
	float Shield = 0.0f;

	//%퍼센트 기준
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival", meta = (DisplayName = "아머 (Armor)", ClampMin = "0.0"))
	float Armor = 0.0f;

	//%퍼센트 기준
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival", meta = (DisplayName = "회피율 (Evasion)", ClampMin = "0.0"))
	float EvasionChance = 0.0f;

	//%퍼센트 기준
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival", meta = (DisplayName = "생명 흡수 (LifeDrain)", ClampMin = "0.0"))
	float LifeDrain = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Survival", meta = (DisplayName = "가시 (Thorn)"))
	float Thorn = 0.0f;

	// ============================================================
	// [공격 (Offense)]
	// ============================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense", meta = (DisplayName = "데미지 배율 (Damage)"))
	float Damage = 0.0f; // 기본 1.0x (StatusComponent 로직에 따라 0이 기본일 수 있음)

	//%퍼센트 기준
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense", meta = (DisplayName = "치명타 확률 (Crit Chance)", ClampMin = "0.0"))
	float CriticalChance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense", meta = (DisplayName = "치명타 데미지 (Crit Dmg)"))
	float PlayerCritDmgRate = 50.0f; // 기본 2.0x (1.5 + 0.5 등 로직 확인 필요)

	//%퍼센트 기준
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Offense", meta = (DisplayName = "공격 속도 (Atk Speed)", ClampMin = "0.0"))
	float AttackSpeed = 0.0f;

	// ============================================================
	// [발사체 (Projectile)]
	// ============================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile", meta = (DisplayName = "발사체 수 (Count)"))
	float ProjectileCount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile", meta = (DisplayName = "발사체 반사 (Reflect)"))
	float ProjectileReflectCount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile", meta = (DisplayName = "크기 (Size)"))
	float AttackSize = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile", meta = (DisplayName = "발사체 속도 (Proj Speed)"))
	float PlayerProjectileSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile", meta = (DisplayName = "지속 시간 (Duration)"))
	float AttackDuration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile", meta = (DisplayName = "엘리트 데미지 (Elite Dmg)"))
	float EliteDamage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Projectile", meta = (DisplayName = "넉백 (Knockback)"))
	float KnockBack = 0.0f;

	// ============================================================
	// [이동 (Movement)]
	// ============================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Movement", meta = (DisplayName = "이동 속도 (MoveSpeed)"))
	float MoveSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Movement", meta = (DisplayName = "추가 점프 (Extra Jump)"))
	float ExtraJump = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Movement", meta = (DisplayName = "점프 높이 (Jump Power)"))
	float JumpPower = 0.0f;
	
	// ============================================================
	// [유틸리티 (Utility)]
	// ============================================================

	//%퍼센트 기준
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility", meta = (DisplayName = "행운 (Luck)", ClampMin = "0.0"))
	float Luck = 0.0f;

	//%퍼센트 기준
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility", meta = (DisplayName = "난이도 (Difficulty)",ClampMin="0.0"))
	float Difficulty = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility", meta = (DisplayName = "픽업 범위 (Pickup Range)"))
	float PickUpRange = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility", meta = (DisplayName = "XP 증가 (Exp Gain)"))
	float ExpGain = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility", meta = (DisplayName = "골드 증가 (Gold Gain)"))
	float GoldGain = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility", meta = (DisplayName = "실버 증가 (Silver Gain)"))
	float SilverGain = 0.0f;

	// 이미지에는 있으나 기존 StatusComponent.h에는 없던 변수 (추가 필요 시 사용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility", meta = (DisplayName = "엘리트 등장 증가 (Elite Spawn Rate)"))
	float EliteSpawnRate = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility", meta = (DisplayName = "파워업 배율 (PowerUp Rate)"))
	float PowerUPRate = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Utility", meta = (DisplayName = "파워업 드롭 확률 (Drop Rate)"))
	float PowerUPDropRate = 0.0f;
};
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UCharacterDataStruct : public UDataTable
{
	GENERATED_BODY()
	
};
