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
