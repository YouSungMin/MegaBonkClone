// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/Weapon.h"
#include "WeaponBase.generated.h"

UCLASS()
class MEGABONKCLONE_API AWeaponBase : public AActor, public IWeapon
{
    GENERATED_BODY()

public:
    AWeaponBase();

    //꼭 구현해야하는 함수들

    //IWeapon 인터페이스 구현부
    virtual void AttackWeapon_Implementation() override {}  //무기마다 다르게 재정의 해야함
    virtual void GetDamageWeapon_Implementation() override;//무기마다 데미지 공식은 똑같을듯
    //오브젝트에 오버랩 되었을때 실행할 함수
    UFUNCTION()
    virtual void OnBeginWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor);

    bool IsValidTarget(AActor* OtherActor);
    //오브젝트에 오버랩 끝났을때 실행할 함수
    UFUNCTION()
    virtual void OnEndWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor);


public:

    //무기의 종류별로 구현 안해도되는 함수들

    // 공격 타이머 시작/재시작 (쿨타임 변경 시 재호출 가능)
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void StartAttackTimer() {}

    // 최종 데미지 계산 (무기 깡뎀 * 플레이어 데미지 배율)
    UFUNCTION(BlueprintPure, Category = "Weapon")
    virtual float GetFinalDamage() const { return 0.0f; }

    // 최종 쿨타임 계산 (무기 쿨타임 * 플레이어 쿨감 배율)
    UFUNCTION(BlueprintPure, Category = "Weapon")
    virtual float GetFinalCooldown() const { return 1.0f; }//임시

    // 최종 범위 계산
    UFUNCTION(BlueprintPure, Category = "Weapon")
    virtual float GetFinalArea() const { return 0.0f; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
    // ==========================================
    // 무기 기본 스탯 (테이블에서 가져올예정)
    // ==========================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float WeaponDamage = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float WeaponCooldown = 1.0f; // 공격 주기

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float WeaponAttackSize = 100.0f; // 범위 혹은 크기

    // ==========================================
    // 내부 변수
    // ==========================================
    UPROPERTY()
    FTimerHandle AttackTimerHandle;

    UPROPERTY()
    TWeakObjectPtr<class UStatusComponent> OwnerStatusComp; // 주인님의 스탯 컴포넌트 캐싱

    UPROPERTY()
    float WeaponFinalDamage = 0.0f;

    UPROPERTY()
    TObjectPtr<UDataAsset> WeaponDataAsset;


};
