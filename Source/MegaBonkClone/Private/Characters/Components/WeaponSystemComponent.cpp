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

	//UI용
	WeaponSlots.Empty();
	WeaponSlots.SetNum(MaxWeaponNum);

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

	/*UE_LOG(LogTemp, Warning, TEXT("[WeaponSystem] AddWeapon Class=%s"),
		InWeapon ? *InWeapon->GetName() : TEXT("NULL"));*/

	//무기 클래스 이름 -> 데이터테이블으로 변환 
	const FName WeaponID = ResolveWeaponIDFromClass(InWeapon);

	//클래스 이름찍기
	/*UE_LOG(LogTemp, Warning, TEXT("[WeaponSystem] AddWeapon Class=%s -> ResolvedID=%s"),
		*InWeapon->GetName(), *WeaponID.ToString());*/


	for (int32 i = 0; i < PlayerWeapons.Num(); i++)
	{
		// 현재 슬롯이 비어있다면?
		if (PlayerWeapons[i] == nullptr)
		{
			PlayerWeapons[i] = InWeapon;

			//UI가 사용할 WeaponID/Level 저장
			WeaponSlots[i].WeaponID = WeaponID;
			WeaponSlots[i].Level = 1;

			OnWeaponChanged.Broadcast();

			UE_LOG(LogWeapon, Log, TEXT("무기 장착 성공: 슬롯 %d"), i);
			EquipWeapon();
			return;
		}
	}

	UE_LOG(LogWeapon, Warning, TEXT("무기 슬롯이 가득 찼습니다! (추가 실패)"));
}

//UI에서 아이콘 이름 가져올떄 사용
FWeaponData* UWeaponSystemComponent::GetWeaponInfo(FName WeaponID) const
{
	if (!WeaponDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponDataTable is not set in WeaponComponent!"));
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
	//DTRowName찍기
	//UE_LOG(LogTemp, Warning, TEXT("[WeaponDT] BeginPlay DT=%s"),
	//	WeaponDataTable ? *WeaponDataTable->GetName() : TEXT("NULL"));

	if (WeaponDataTable)
	{
		const TArray<FName> Names = WeaponDataTable->GetRowNames();
		for (const FName& N : Names)
		{
			UE_LOG(LogTemp, Warning, TEXT("[WeaponDT] RowName=%s"), *N.ToString());
		}
	}
	//

	UE_LOG(LogTemp, Warning, TEXT("[WeaponSystem] BeginPlay called"));

	// 시작 시 PlayerWeapons -> WeaponSlots 동기화
	for (int32 i = 0; i < MaxWeaponNum; ++i)
	{
		if (!PlayerWeapons.IsValidIndex(i) || PlayerWeapons[i] == nullptr)
		{
			WeaponSlots[i].WeaponID = NAME_None;
			WeaponSlots[i].Level = 0;
			continue;
		}

		WeaponSlots[i].WeaponID = ResolveWeaponIDFromClass(PlayerWeapons[i]);
		WeaponSlots[i].Level = 1;
	}

	OnWeaponChanged.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("[WeaponSystem] Broadcast OnWeaponChanged"));
}


//무기클래스 이름 -> DTRow로 변환함 
FName UWeaponSystemComponent::ResolveWeaponIDFromClass(TSubclassOf<AActor> InWeapon) const
{
	if (!InWeapon) return NAME_None;

	FString Name = InWeapon->GetName();

	// 1) BP_ 접두어 제거
	Name.RemoveFromStart(TEXT("BP_"));

	// 2) _C 접미어 제거
	Name.RemoveFromEnd(TEXT("_C"));

	// 3) 첫 글자만 대문자 (RowName이 Flamewalker 형태라면)
	if (Name.Len() > 0)
	{
		Name[0] = FChar::ToUpper(Name[0]);
	}

	const FName Candidate(*Name);

	// 4) 진짜로 DT에 Row가 있으면 그걸 반환
	if (WeaponDataTable->FindRow<FWeaponData>(Candidate, TEXT("ResolveWeaponIDFromClass")))
	{
		return Candidate;
	}

	return NAME_None;
}



