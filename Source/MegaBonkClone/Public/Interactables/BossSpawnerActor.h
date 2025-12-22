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

	// 상호작용 인터페이스 구현
	virtual void Interact_Implementation(AActor* PlayerActor) override;

private:
	// 보스가 다처치되었을 실행될 함수
	UFUNCTION()
	void OnBossClear();

	// 보스를 파라미터의 값만큼 소환하는 함수
	void SpawnBosses(int32 Amount);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	// 보스 스포너 외형
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> BossSpawnerMesh;

	// 게임 스테이트 베이스 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AMegaBonkGameState> GameState = nullptr;
private:
	// 현재 보스 스포너의 상태 
	EAltarState CurrentState = EAltarState::ReadyToSummon;
};
