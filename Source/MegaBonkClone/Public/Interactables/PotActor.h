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

	virtual void Interact_Implementation(AActor* InstigatorActor) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> PotMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pot|Coin")
	TSubclassOf<AActor> CoinClass;

	// 깨질 때 나올 이펙트 (파티클)
	UPROPERTY(EditAnywhere, Category = "FX")
	TObjectPtr<UParticleSystem> BreakEffect = nullptr;

	// 깨질 때 날 소리
	UPROPERTY(EditAnywhere, Category = "FX")
	TObjectPtr<USoundBase> BreakSound = nullptr;
};
