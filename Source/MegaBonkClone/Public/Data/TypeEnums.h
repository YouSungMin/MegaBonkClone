// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBuffType : uint8
{
    SpeedUp,        // 이동 속도 증가
    AttackSpeed,    // 공격 속도 증가
    GoldBoost       // 골드 획득량 증가
};

// 아이템 종류
UENUM(BlueprintType)
enum class EItemType : uint8
{
	GeneralItem		UMETA(DisplayName = "일반 아이템"), // 갯수 제한 없이 중첩 가능
	SecretBook		UMETA(DisplayName = "비전서"), // 최대 4개까지 장착 가능
	Weapon			UMETA(DisplayName = "무기") // 최대 4개까지 장착 가능
};

// 아이템 등급
UENUM(BlueprintType)
enum class EItemGrade : uint8
{
	None,
	Common		UMETA(DisplayName = "커먼 (Common)"),
	Rare		UMETA(DisplayName = "레어 (Rare)"),
	Epic		UMETA(DisplayName = "에픽 (Epic)"),
	Legendary	UMETA(DisplayName = "전설 (Legendary)")
};

// 스탯 유형
UENUM(BlueprintType)
enum class EItemStatType : uint8
{
	None					UMETA(DisplayName = "없음"),
	MaxHP					UMETA(DisplayName = "최대 체력"),
	HPRegen					UMETA(DisplayName = "체력 재생"),
	OverHeal				UMETA(DisplayName = "오버 힐"),
	Shield					UMETA(DisplayName = "실드"),
	Armor					UMETA(DisplayName = "방어력"),
	EvasionChance			UMETA(DisplayName = "회피율"),
	LifeDrain				UMETA(DisplayName = "생명 흡수"),
	Thorn					UMETA(DisplayName = "가시"),
	ExpGain					UMETA(DisplayName = "경험치 획득량"),
	SilverGain				UMETA(DisplayName = "실버 획득량"),
	GoldGain				UMETA(DisplayName = "골드 획득량"),
	PickUpRange				UMETA(DisplayName = "픽업 범위"),
	Difficulty				UMETA(DisplayName = "난이도"),
	Damage					UMETA(DisplayName = "데미지"),
	CritChance				UMETA(DisplayName = "치명타 확률"),
	MegaCritChance			UMETA(DisplayName = "메가 치명타 확률"),
	AttackSpeed				UMETA(DisplayName = "공격속도"),
	ProjectileCount			UMETA(DisplayName = "발사체 수"),
	ProjectileReflectCount	UMETA(DisplayName = "발사체 반사 수"),
	AttackSize				UMETA(DisplayName = "발사체 크기"),
	ProjectileSpeed			UMETA(DisplayName = "발사체 속도"),
	AttackDuration			UMETA(DisplayName = "지속 시간"),
	KnockBack				UMETA(DisplayName = "넉백"),
	MoveSpeed				UMETA(DisplayName = "이동 속도"),
	JumpPower				UMETA(DisplayName = "점프 높이"),
	ExtraJump				UMETA(DisplayName = "점프 횟수"),
	Luck					UMETA(DisplayName = "행운")
};

// 특수 효과 유형
UENUM(BlueprintType)
enum class EProcEffectType : uint8
{
	None					UMETA(DisplayName = "없음"),
	SpawnActor				UMETA(DisplayName = "액터 소환"),
	FireProjectile			UMETA(DisplayName = "투사체 발사"),
	ApplyStatus				UMETA(DisplayName = "상태 이상 부여"),
	AreaExplosion			UMETA(DisplayName = "범위 공격"),
	ReflectDamage			UMETA(DisplayName = "반사"),
	TimeStop				UMETA(DisplayName = "시간 정지"),
	Execute					UMETA(DisplayName = "처형"),
	FreeInteract			UMETA(DisplayName = "상자 무료"),
	TemporaryBuffStat		UMETA(DisplayName = "일시적 스탯 버프"),
	PermanentBuffStat		UMETA(DisplayName = "영구적 스탯 버프")
};

UENUM(BlueprintType)
enum class EStatusEffectType : uint8
{
	None,
	Freeze		UMETA(DisplayName = "빙결"),
	Poison		UMETA(DisplayName = "독/중독"),
	Burn		UMETA(DisplayName = "화상"),
	Shock		UMETA(DisplayName = "감전"),
	Bleed		UMETA(DisplayName = "출혈"),
	Curse		UMETA(DisplayName = "저주")
};

UENUM(BlueprintType)
enum class EProcTriggerType : uint8
{
	OnHit			UMETA(DisplayName = "공격 시"),
	OnKill			UMETA(DisplayName = "처치 시"),
	OnCritical		UMETA(DisplayName = "치명타 피해 시"),
	OnInteract		UMETA(DisplayName = "상호작용 시"),
	OnTakeDamage	UMETA(DisplayName = "피격 시"),
	OnTick			UMETA(DisplayName = "매 프레임"),
	OnEvade         UMETA(DisplayName = "회피 성공 시"),
	OnOpenChest		UMETA(DisplayName = "상자를 열때"),
	OnJump			UMETA(DisplayName = "점프 시"),
	OnMove			UMETA(DisplayName = "이동 시"),
	OnLifeDrain		UMETA(DisplayName = "생명 흡수 시"),
	OnHeal			UMETA(DisplayName = "체력 회복 시"),
	OnLowHP			UMETA(DisplayName = "체력이 50% 이하일때")
};
UENUM(BlueprintType)
enum class EAltarState : uint8
{
	ReadyToSummon, // 처음 상태
	BossesAlive,   // 보스 잡는 중 (상호작용 불가)
	StageClear     // 다 잡음 (다음 스테이지 이동)
};

// 무기 강화 선택지 목록
UENUM(BlueprintType)
enum class EWeaponStatType : uint8
{
	None,
	Damage			UMETA(DisplayName = "데미지"),
	ProjectileCount UMETA(DisplayName = "발사체 수"),
	ProjectileSpeed UMETA(DisplayName = "발사체 속도"),
	ProjectileScale UMETA(DisplayName = "발사체 크기(범위)"),
	ChainCount		UMETA(DisplayName = "반사 횟수"),
	CritChance		UMETA(DisplayName = "치명타 확률"),
	CritDamage		UMETA(DisplayName = "치명타 데미지"),
	KnockBack		UMETA(DisplayName = "넉백"),
	Cooldown		UMETA(DisplayName = "쿨타임 감소")
};

// 전자레인지 상태 열거형 정의
UENUM(BlueprintType)
enum class EMicrowaveState : uint8
{
	Ready       UMETA(DisplayName = "준비됨 (사용 가능)"),
	Cooking     UMETA(DisplayName = "조리 중"),
	Completed   UMETA(DisplayName = "조리 완료 (수령 대기)")
};