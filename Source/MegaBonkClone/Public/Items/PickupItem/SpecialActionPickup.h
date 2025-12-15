// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PickupItem/PickupBaseActor.h"
#include "SpecialActionPickup.generated.h"
UENUM(BlueprintType)
enum class ESpecialActionType : uint8
{
	Nuke,       // 화면 전체 적 제거
	Magnet,     // 모든 경험치/골드 흡수
};
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API ASpecialActionPickup : public APickupBaseActor
{
	GENERATED_BODY()
protected:
	virtual void OnPickupComplete_Implementation() override;

private:
	void ExecuteNuke();
	void ExecuteMagnet();
public:
	// 에디터에서 무슨 아이템인지 선택
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Action")
	ESpecialActionType ActionType;

};
