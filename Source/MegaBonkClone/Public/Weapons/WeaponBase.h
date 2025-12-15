// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/Weapon.h"
#include "WeaponBase.generated.h"
struct FWeaponData;

UCLASS()
class MEGABONKCLONE_API AWeaponBase : public AActor, public IWeapon
{
    GENERATED_BODY()

public:
    AWeaponBase();
 

    //IWeapon 인터페이스 구현부
    //무기 공격 함수
    virtual void AttackWeapon_Implementation() override;  //무기마다 다르게 재정의 해야함

    //무기데미지 계산
    virtual void GetDamageWeapon_Implementation() override;//무기마다 데미지 공식은 똑같을듯

    //액터 유효성 검사 //플레이어인지 아닌지  , 유효한지 아닌지
    bool IsValidTarget(AActor* OtherActor);
   

public:

    //몬스터에 적용할 최종 데미지 함수
    // 최종 데미지 계산 (무기 깡뎀 * 플레이어 데미지 배율)
    UFUNCTION(BlueprintPure, Category = "Weapon")
    virtual float GetFinalDamage() const;


    //공격속도 영향
    //공격 타이머 시작/재시작 (쿨타임 변경 시 재호출 가능)
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void StartAttackTimer();

    UFUNCTION()
    void InitializeWeaponStatus(const FWeaponData& InWeaponData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    void LoadWeaponData();

    //타이머에서 AttackWeapon함수 바로 호출 불가능해서 만든 인보크 함수
    UFUNCTION()
    void InvokeAttack();

public:
    // ==========================================
    // 무기 기본 스탯 (테이블에서 가져올예정)
    // ==========================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float WeaponDamage = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float ProjectileCount = 1.0f; // 발사체 수

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float ProjectileSpeed = 1.0f; // 발사체 속도

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float ProjectileAttackSize = 1.0f; // 발사체 크기

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float ProjectileReflectCount = 1.0f; // 발사체 반사

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float CriticalChance = 1.0f; // 치명타 확률

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float CritDmgRate = 1.0f; // 치명타 데미지

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float KnockBack = 1.0f; // 넉백

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Settings", meta = (RowType = "/Script/MegaBonkClone.WeaponData"))
    FDataTableRowHandle WeaponTableRow;

    // ==========================================
    // 내부 변수
    // ==========================================
public:

    UPROPERTY()
    FTimerHandle AttackTimerHandle;

    UPROPERTY()
    TWeakObjectPtr<class UStatusComponent> OwnerStatusComp; // 주인님의 스탯 컴포넌트 캐싱

    UPROPERTY()
    float WeaponFinalDamage = 0.0f;


};
