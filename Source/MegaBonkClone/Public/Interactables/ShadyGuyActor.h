// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/TypeEnums.h"
#include "Interfaces/InteractionInterface.h"
#include "ShadyGuyActor.generated.h"

// UI에 보낼 상점 슬롯 정보
USTRUCT(BlueprintType)
struct FShopSlotInfo
{
	GENERATED_BODY()

	// 1. 로직용 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Data")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Data")
	int32 Price;

	// 2. UI 표시용 데이터 (추가된 부분)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Display")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Display")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Display")
	EItemGrade ItemGrade;

	// UI 이미지 브러시에 바로 넣기 위해 SoftPtr 대신 실제 로드된 텍스처 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Display")
	UTexture2D* IconTexture;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShopOpen, const TArray<FShopSlotInfo>&, ShopItems, AShadyGuyActor*, MerchantInstance);
UCLASS()
class MEGABONKCLONE_API AShadyGuyActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShadyGuyActor();

	// UI가 구독할 이벤트 (블루프린트 할당 가능)
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnShopOpen OnShopOpen;

	// UI에서 구매 버튼을 눌렀을 때 호출될 실제 처리 함수
	UFUNCTION(BlueprintCallable, Category = "Shop")
	void ProcessPurchase(int32 ItemIndex);

protected:
	virtual void Tick(float DeltaTime)override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact_Implementation(AActor* PlayerActor) override;
private:
	// 확률에 따라 등급을 결정하는 함수
	void DetermineRarity();

	// 결정된 등급에 따라 텍스처를 입히는 함수
	void UpdateMeshTexture();

	// 데이터 테이블에서 아이템을 뽑아오는 함수
	void GenerateShopItems();
protected:
	// 기준점 더미 루트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	// 수상한녀석 외형
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> ShadyGuyMesh;

	// 에디터에서 등급별 텍스처를 지정할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadyGuy Textures")
	UTexture2D* CommonTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadyGuy Textures")
	UTexture2D* RareTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadyGuy Textures")
	UTexture2D* EpicTexture;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadyGuy Textures")
	UTexture2D* LegendaryTexture;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShadyGuy Properties")
	EItemGrade CurrentRarity;

	// 에디터에서 설정할 등급 별 확률 가중치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadyGuy Config")
	TMap<EItemGrade, float> RarityDropRates;

	// 아이템 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadyGuy Data")
	UDataTable* ItemDataTable;

	// 이번에 뽑힌 아이템 3개를 저장할 리스트
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Shop")
	TArray<FShopSlotInfo> CurrentShopItems;
private:
	bool bIsUsed = false;

	UPROPERTY()
	TWeakObjectPtr<AActor> Player;
};
