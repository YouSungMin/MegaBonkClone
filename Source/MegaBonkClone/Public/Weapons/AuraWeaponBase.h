// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/WeaponBase.h"
#include "AuraWeaponBase.generated.h"

UCLASS()
class MEGABONKCLONE_API AAuraWeaponBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAuraWeaponBase();

	//IWeapon 인터페이스 구현부
	virtual void AttackWeapon_Implementation() override;

	// 오라 크기를 업데이트하는 함수 (스탯 변화 시 호출 가능)
	UFUNCTION(BlueprintCallable, Category = "Aura")
	void UpdateAuraScale();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Aura|Component")
	TObjectPtr<UStaticMeshComponent> BaseMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Aura|Component")
	TObjectPtr<UStaticMeshComponent>  RingMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Aura|Component")
	TObjectPtr<class UCapsuleComponent> Collision = nullptr;


};
