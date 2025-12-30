// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/SanctuaryBase.h"
#include "Data/TypeEnums.h"
#include "MoaiSanctuary.generated.h"

/**
 * 
 */
 // UI에 보낼 아이템 슬롯 정보
USTRUCT(BlueprintType)
struct FMoaiSlotInfo
{
	GENERATED_BODY()

	// 1. 로직용 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moai Data")
	FName ItemID;

	// 2. UI 표시용 데이터 (추가된 부분)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moai Display")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moai Display")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moai Display")
	EItemGrade ItemGrade;

	// UI 이미지 브러시에 바로 넣기 위해 SoftPtr 대신 실제 로드된 텍스처 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moai Display")
	UTexture2D* IconTexture;
};

UCLASS()
class MEGABONKCLONE_API AMoaiSanctuary : public ASanctuaryBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void Interact_Implementation(AActor* PlayerActor) override;
private:
	// 확률에 기반해 아이템 3개를 생성하는 함수
	void GenerateMoaiItems();

	// 확률 가중치에 따라 등급 하나를 반환
	EItemGrade GetRandomRarity();
protected:
	// 에디터에서 설정할 등급 별 확률 가중치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadyGuy Config")
	TMap<EItemGrade, float> RarityDropRates;

	// 아이템 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadyGuy Data")
	UDataTable* ItemDataTable;

	// 이번에 뽑힌 아이템 3개를 저장할 리스트
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Shop")
	TArray<FMoaiSlotInfo> CurrentMoaiItems;
private:
	UPROPERTY()
	TWeakObjectPtr<AActor> Player;
};
