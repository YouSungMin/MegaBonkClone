// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "SanctuaryBase.generated.h"

UCLASS()
class MEGABONKCLONE_API ASanctuaryBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASanctuaryBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact_Implementation(AActor* PlayerActor) override;
protected:	
	// 기준점 더미 루트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	// 성소 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SantuaryMesh;

	UFUNCTION(BlueprintNativeEvent, Category = "Sanctuary")
	void ApplyEffect(AActor* Player);

	virtual void ApplyEffect_Implementation(AActor* Player);
protected:
	bool bIsUsed = false;
};
