// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/WeaponDataStructs.h"
#include "WeaponSystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponChanged);

USTRUCT(BlueprintType)
struct FWeaponSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 1;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEGABONKCLONE_API UWeaponSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponSystemComponent();

	//배열에 있는 무기 액터에 스폰후 어태치
	UFUNCTION(BlueprintCallable)
	void EquipWeapon();

	UFUNCTION(BlueprintCallable)
	void AddWeapon(TSubclassOf<AActor> InWeapon);

	//DT에서 무기 정보 가져오기 
	FWeaponData* GetWeaponInfo(FName WeaponID) const; 

	//UI가 슬롯정보 접근할수있게 
	UFUNCTION(BlueprintPure, Category = "Weapon")
	const TArray<TSubclassOf<AActor>>& GetWeaponSlots() const { return PlayerWeapons; }

	//바인딩
	UPROPERTY(BlueprintAssignable)
	FOnWeaponChanged OnWeaponChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//DT지정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	TObjectPtr <class UDataTable> WeaponDataTable = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WeaponComponent|Weapons")
	TArray<TSubclassOf<AActor>> PlayerWeapons;

private:
	//무기 슬롯 크기
	int32 MaxWeaponNum = 3;
};
