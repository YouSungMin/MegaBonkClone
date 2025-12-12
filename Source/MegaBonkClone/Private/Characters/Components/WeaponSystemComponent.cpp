// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/WeaponSystemComponent.h"
#include "Weapons/WeaponBase.h"
#include "GameFramework/Character.h"
// Sets default values for this component's properties
UWeaponSystemComponent::UWeaponSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	//고정 무기 슬롯
	int32 MaxWeaponNum = 3;
	PlayerWeapons.Empty();
	PlayerWeapons.SetNum(MaxWeaponNum);

}

void UWeaponSystemComponent::EquipWeapon()
{
	ACharacter* character = Cast<ACharacter>(GetOwner());
	for (const TSubclassOf<AActor> elements : PlayerWeapons) {
		if (elements) {
			AActor* spawnWeapon = GetWorld()->SpawnActor<AActor>(elements, FVector::ZeroVector, FRotator::ZeroRotator);
			spawnWeapon->AttachToComponent(character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
		}
	}
}


// Called when the game starts
void UWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...


	
}



