// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/Weapon.h"
#include "WeaponBase.generated.h"

UCLASS()
class MEGABONKCLONE_API AWeaponBase : public AActor , public IWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	//IWeapon 인터페이스 구현부
	virtual void AttackWeapon_Implementation() override;
	virtual void GetDamageWeapon_Implementation() override;

    // 공격 타이머 시작/재시작 (쿨타임 변경 시 재호출 가능)
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void StartAttackTimer();

    // 최종 데미지 계산 (무기 깡뎀 * 플레이어 데미지 배율)
    UFUNCTION(BlueprintPure, Category = "Weapon")
    float GetFinalDamage() const;

    // 최종 쿨타임 계산 (무기 쿨타임 * 플레이어 쿨감 배율)
    UFUNCTION(BlueprintPure, Category = "Weapon")
    float GetFinalCooldown() const;

    // 최종 범위 계산
    UFUNCTION(BlueprintPure, Category = "Weapon")
    float GetFinalArea() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
    // ==========================================
    // 무기 기본 스탯
    // ==========================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float BaseDamage = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float BaseCooldown = 1.0f; // 공격 주기

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float BaseArea = 100.0f; // 범위 혹은 크기

    // ==========================================
    // 내부 변수
    // ==========================================
    //UPROPERTY()
    //FTimerHandle AttackTimerHandle;

    //UPROPERTY()
   // class UStatusComponent* OwnerStatusComp; // 주인님의 스탯 컴포넌트 캐싱

};
