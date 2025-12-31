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


	ActiveSlots.Empty();
	ActiveSlots.SetNum(MaxWeaponSlots);

}

void UWeaponSystemComponent::AddWeapon(FName WeaponID)
{
	if (!WeaponDataTable) return;

	//이미 가지고 있는지 체크
	if (HasWeapon(WeaponID)) return;

	//데이터 조회
	FWeaponData* Row = WeaponDataTable->FindRow<FWeaponData>(WeaponID, TEXT("AddWeapon"));
	if (!Row || !Row->WeaponClass) return;

	//빈 슬롯 찾기
	for (auto& Slot : ActiveSlots)
	{
		if (Slot.IsEmpty()) // 비어있다면
		{
			//실제 액터 스폰
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();

			AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(Row->WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

			if (NewWeapon)
			{
				//캐릭터에 부착
				ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
				if (OwnerChar)
				{
					NewWeapon->AttachToComponent(OwnerChar->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
				}

				//슬롯 데이터 채우기 (통합!)
				Slot.WeaponID = WeaponID;
				Slot.Level = 1;
				Slot.WeaponInstance = NewWeapon; // 인스턴스 저장

				//UI 알림
				OnWeaponChanged.Broadcast();

				UE_LOG(LogTemp, Log, TEXT("[WeaponSystem] 무기 추가 완료: %s"), *WeaponID.ToString());
			}
			return; //추가했으니 종료
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[WeaponSystem] 슬롯이 가득 찼습니다."));
}

void UWeaponSystemComponent::LevelUpWeapon(FName WeaponID, EItemStatType StatType, float IncreaseValue)
{
	// 슬롯을 순회하며 내 무기 찾기
	for (auto& Slot : ActiveSlots)
	{
		if (Slot.WeaponID == WeaponID)
		{
			// 1. 레벨 증가
			Slot.Level++;

			// 2. 실제 무기 스탯 적용 (인스턴스가 있으니 바로 접근 가능!)
			if (Slot.WeaponInstance)
			{
				// AWeaponBase에 스탯 적용 함수가 있다고 가정 (없으면 변수 직접 수정)
				// Slot.WeaponInstance->ApplyUpgrade(StatType, IncreaseValue); 
				switch (StatType)
				{
				case EItemStatType::Damage:
					Slot.WeaponInstance->WeaponDamage += IncreaseValue;
					break;
				case EItemStatType::CritChance:
					Slot.WeaponInstance->CriticalChance += IncreaseValue;
					break;
				case EItemStatType::ProjectileCount:
					Slot.WeaponInstance->ProjectileCount += IncreaseValue;
					break;
				case EItemStatType::ProjectileReflectCount:
					Slot.WeaponInstance->ProjectileReflectCount += IncreaseValue;
					break;
				case EItemStatType::AttackSize:
					Slot.WeaponInstance->AttackSize += IncreaseValue;
					break;
				case EItemStatType::ProjectileSpeed:
					Slot.WeaponInstance->ProjectileSpeed += IncreaseValue;
					break;
				case EItemStatType::KnockBack:
					Slot.WeaponInstance->KnockBack += IncreaseValue;
					break;
				default:
					break;
				}
			}

			OnWeaponChanged.Broadcast();
			return;
		}
	}
}

// ================= Helpers =================

bool UWeaponSystemComponent::HasWeapon(FName WeaponID) const
{
	for (const auto& Slot : ActiveSlots)
	{
		if (Slot.WeaponID == WeaponID) return true;
	}
	return false;
}

int32 UWeaponSystemComponent::GetWeaponLevel(FName WeaponID) const
{
	for (const auto& Slot : ActiveSlots)
	{
		if (Slot.WeaponID == WeaponID) return Slot.Level;
	}
	return 0;
}

int32 UWeaponSystemComponent::GetCurrentWeaponCount() const
{
	int32 Count = 0;
	for (const auto& Slot : ActiveSlots)
	{
		if (!Slot.IsEmpty()) Count++;
	}
	return Count;
}

FWeaponData* UWeaponSystemComponent::GetWeaponInfo(FName WeaponID) const
{
	//데이터 테이블이 없거나 ID가 없으면 빈 값 반환
	if (!WeaponDataTable || WeaponID.IsNone()) return nullptr;

	//데이터 테이블에서 ID로 검색해서 반환
	static const FString ContextString(TEXT("GetWeaponInfo"));
	return WeaponDataTable->FindRow<FWeaponData>(WeaponID, ContextString);
}


FName UWeaponSystemComponent::ResolveWeaponIDFromClass(TSubclassOf<AActor> InWeaponClass) const
{
	//유효성 검사 (입력값 및 DT 확인)
	if (!InWeaponClass || !WeaponDataTable) return NAME_None;

	//데이터 테이블의 모든 행(Row) 이름 가져오기
	TArray<FName> RowNames = WeaponDataTable->GetRowNames();

	//반복문을 돌면서 클래스 비교 (역참조 방식)
	for (const FName& RowName : RowNames)
	{
		//해당 행의 데이터 가져오기
		FWeaponData* RowData = WeaponDataTable->FindRow<FWeaponData>(RowName, TEXT("ResolveWeaponID"));

		//데이터가 유효하고, 클래스가 일치하는지 확인
		if (RowData && RowData->WeaponClass == InWeaponClass)
		{
			//일치하는 행의 ID(RowName) 반환
			return RowName;
		}
	}

	//일치하는 무기가 없으면 None 반환
	return NAME_None;
}

void UWeaponSystemComponent::Debug_TestWeapon(FName WeaponID)
{
	if (WeaponID.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("[Debug] 테스트할 무기 ID가 입력되지 않았습니다 (None)."));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("=========== [Weapon Test Logic] ==========="));

	//이미 가지고 있는지 확인
	if (HasWeapon(WeaponID))
	{
		// [강화] 가지고 있다면 -> 데미지 +5.0 강화 테스트
		UE_LOG(LogTemp, Log, TEXT("[Debug] 이미 보유 중인 무기입니다. 강화를 시도합니다: %s"), *WeaponID.ToString());

		// EItemStatType::Damage는 프로젝트의 Enum에 정의된 값이어야 합니다.
		// 만약 다른 스탯을 테스트하고 싶다면 파라미터로 받게 수정해도 됩니다.
		LevelUpWeapon(WeaponID, EItemStatType::Damage, 5.0f);
	}
	else
	{
		// [신규] 없다면 -> 신규 장착 테스트
		UE_LOG(LogTemp, Log, TEXT("[Debug] 보유하지 않은 무기입니다. 신규 추가를 시도합니다: %s"), *WeaponID.ToString());

		AddWeapon(WeaponID);
	}

	UE_LOG(LogTemp, Warning, TEXT("==========================================="));
}
