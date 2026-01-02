// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Data/ItemDataStructs.h"
#include "ChestActor.generated.h"
struct FLootCandidate
{
	FName ItemID;
	float Weight;
};

UCLASS()
class MEGABONKCLONE_API AChestActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact_Implementation(AActor* PlayerActor) override;
	virtual void BeginFocus_Implementation() override;
	virtual void EndFocus_Implementation() override;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	FName GetRandomItemID();

	void ChestNotifyToHUD(FString Message);
protected:
	//띄울 상호작용 키 위젯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UWidgetComponent> InteractionWidgetComp;
	// 등급별 등장 가중치 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Settings")
	TMap<EItemGrade, float> TierProbabilities;

	// 전체 아이템 데이터 테이블
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> ItemDataTable;

	// 상자 외형
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ChestMesh;

	// 상자 여는 비용 (0이면 무료, 100이면 100골드 필요)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Settings")
	float OpenCost = 0;


	// 오버랩 감지용 트리거 박스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UBoxComponent> TriggerBox;

	// 이미 열렸는지 확인하는 플래그 (중복 실행 방지)
	bool bIsOpened = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Settings")
	bool bAutoOpen = false;
};
