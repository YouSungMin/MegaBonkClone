// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PickupItem/PickupBaseActor.h"
#include "BuffPickup.generated.h"

UENUM(BlueprintType)
enum class EBuffType : uint8 
{ 
    SpeedUp,        // 이동 속도 증가
    Invincible,     // 일정시간 무적 
    AttackSpeed,    // 공격 속도 증가
    GoldBoost,      // 골드 획득량 증가
    Stopwatch       // 일정 시간 적 정지
};
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API ABuffPickup : public APickupBaseActor
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
    EBuffType BuffType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
    float BuffDuration = 5.0f;

    virtual void OnPickupComplete_Implementation() override;
};
