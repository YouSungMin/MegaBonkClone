// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Data/TypeEnums.h"
#include "MicrowaveActor.generated.h"

USTRUCT(BlueprintType)
struct FMicrowaveSlotInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(BlueprintReadOnly)
	FText ItemName;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentQuantity;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMicrowaveOpen, const TArray<FMicrowaveSlotInfo>&, FilteredItems, AMicrowaveActor*, MicrowaveActor);
UCLASS()
class MEGABONKCLONE_API AMicrowaveActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMicrowaveActor();

	// UI 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMicrowaveOpen OnMicrowaveOpen;

	// UI에서 아이템 선택 시 호출할 함수
	UFUNCTION(BlueprintCallable, Category = "Microwave")
	void StartCooking(FName SelectedItemID);

	UFUNCTION(BlueprintPure, Category = "Microwave")
	int32 GetGoldCost() const { return GoldCost; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Interact_Implementation(AActor* PlayerActor) override;
private:
	void DetermineRarity();		// 등급 결정
	void UpdateMeshTexture();	// 외형 적용

	UFUNCTION()
	void OnCookingFinished();

	// 완료된 아이템 수령 및 페널티 처리 함수
	void RetrieveResult();
 protected:
	 // 기준점 더미 루트
	 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	 USceneComponent* Root;

	// 전자레인지 외형
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MicrowaveMesh;

	// 에디터에서 등급별 텍스처를 지정할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Microwave Textures")
	UTexture2D* CommonTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Microwave Textures")
	UTexture2D* RareTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Microwave Textures")
	UTexture2D* EpicTexture;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Microwave Textures")
	UTexture2D* LegendaryTexture;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Microwave Properties")
	EItemGrade CurrentRarity;

	// 에디터에서 설정할 등급 별 확률 가중치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Microwave Config")
	TMap<EItemGrade, float> RarityDropRates;

	// 현재 전자레인지 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Microwave State")
	EMicrowaveState CurrentState = EMicrowaveState::Ready;

	// 사용 시 필요한 골드 비용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Microwave Config")
	int32 GoldCost = 500;

	// 조리에 걸리는 시간 (초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Microwave Config")
	float CookTime = 3.0f;
private:
	FTimerHandle CookingTimerHandle;
	FName SavedItemID;
	TWeakObjectPtr<AActor> Player;
	TObjectPtr<class UInventoryComponent> Inventory = nullptr;
};
