// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/WeaponSystemComponent.h"
#include "Weapons/WeaponBase.h"
// Sets default values for this component's properties
UWeaponSystemComponent::UWeaponSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	//고정 무기 슬롯
	int MaxWeaponNum = 3;
	PlayerWeapons.Empty(MaxWeaponNum);

}

void UWeaponSystemComponent::EquipWeapon()
{
	
	for (const TSubclassOf<class AWeaponBase> elements : PlayerWeapons) {
		if (elements) {
			AWeaponBase* spawnWeapon = GetWorld()->SpawnActor<AWeaponBase>(elements, FVector::ZeroVector, FRotator::ZeroRotator);
			spawnWeapon->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetIncludingScale, NAME_None);
		}
	}
}


// Called when the game starts
void UWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...


	
}



