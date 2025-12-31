// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "PotActor.generated.h"

UCLASS()
class MEGABONKCLONE_API APotActor : public AActor , public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APotActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// InteractionInterface 구현
	virtual void Interact_Implementation(AActor* InstigatorActor) override;


	virtual void BeginFocus_Implementation() override;
	virtual void EndFocus_Implementation() override;
protected:
	//상호작용키
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UWidgetComponent> InteractionWidgetComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> PotMesh = nullptr;

	// 깨질 때 나오는 아이템 클래스 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pot|Drops")
	TArray<TSubclassOf<class AResourcePickup>> DropPickup;

	// 아이템이 나오는 최소 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pot|Drops")
	int32 MinDropCount = 3;

	// 아이템이 나오는 최대 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pot|Drops")
	int32 MaxDropCount = 5;

	// 깨질 때 나올 이펙트 (파티클)
	UPROPERTY(EditAnywhere, Category = "FX")
	TObjectPtr<UParticleSystem> BreakEffect = nullptr;

	// 깨질 때 날 소리
	UPROPERTY(EditAnywhere, Category = "FX")
	TObjectPtr<USoundBase> BreakSound = nullptr;
};
