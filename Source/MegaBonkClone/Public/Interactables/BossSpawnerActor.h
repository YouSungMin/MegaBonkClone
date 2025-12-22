// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Data/TypeEnums.h"
#include "BossSpawnerActor.generated.h"

UCLASS()
class MEGABONKCLONE_API ABossSpawnerActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossSpawnerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact_Implementation(AActor* PlayerActor) override;

	UFUNCTION(BlueprintCallable)
	void OnBossClear();
private:
	void SpawnBosses(int32 Amount);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	// 전자레인지 외형
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> BossSpawnerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AMegaBonkGameState> GameState = nullptr;
private:
	EAltarState CurrentState = EAltarState::ReadyToSummon;
};
