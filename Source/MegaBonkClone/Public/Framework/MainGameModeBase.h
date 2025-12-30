// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Setup")
	TArray<TSubclassOf<class ACharacter>> CharacterOptions;

	//defaltpawn 랜덤선택 함수
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

};
