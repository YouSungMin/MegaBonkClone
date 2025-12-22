// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Data/TypeEnums.h"
#include "MicrowaveActor.generated.h"

UCLASS()
class MEGABONKCLONE_API AMicrowaveActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMicrowaveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Interact_Implementation(AActor* PlayerActor) override;
 protected:
	 // 기준점 더미 루트
	 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	 USceneComponent* Root;

	// 전자레인지 외형
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MicrowaveMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties")
	EItemGrade CurrentRarity;
};
