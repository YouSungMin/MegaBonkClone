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
	virtual void GetDamageWeapon_Implementation() override;

	virtual void StartAttackTimer() override;

	//오브젝트에 오버랩 되었을때 실행할 함수
	virtual void OnBeginWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor) override;

	//오브젝트에 오버랩 끝났을때 실행할 함수 
	virtual void OnEndWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor) override;

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
