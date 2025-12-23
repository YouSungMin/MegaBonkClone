// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/WeaponSystemComponent.h"
#include "Weapons/WeaponBase.h"
#include "MegaBonkClone/MegaBonkClone.h"
#include "GameFramework/Character.h"
// Sets default values for this component's properties
UWeaponSystemComponent::UWeaponSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	//고정 무기 슬롯
	
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

void UWeaponSystemComponent::AddWeapon(TSubclassOf<AActor> InWeapon)
{
	if (!InWeapon) return;

	for (int32 i = 0; i < PlayerWeapons.Num(); i++)
	{
		// 현재 슬롯이 비어있다면?
		if (PlayerWeapons[i] == nullptr)
		{
			PlayerWeapons[i] = InWeapon;
			UE_LOG(LogWeapon, Log, TEXT("무기 장착 성공: 슬롯 %d"), i);
			EquipWeapon();
			return;
		}
	}

	UE_LOG(LogWeapon, Warning, TEXT("무기 슬롯이 가득 찼습니다! (추가 실패)"));
	OnWeaponChanged.Broadcast();
}

FWeaponData* UWeaponSystemComponent::GetWeaponInfo(FName WeaponID) const
{
	if (!WeaponDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponDataTable is not set in InventoryComponent!"));
		return nullptr;
	}
	static const FString ContextString(TEXT("GetWeaponInfo"));
	
	return WeaponDataTable->FindRow<FWeaponData>(WeaponID, ContextString);

}


// Called when the game starts
void UWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}



