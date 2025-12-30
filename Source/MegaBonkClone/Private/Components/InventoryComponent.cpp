// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Characters/Components/StatusComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


}


void UInventoryComponent::ProcessProcTrigger(EProcTriggerType Trigger, AActor* TargetActor, float TriggerValue)
{
	// 비전서는 특수 효과가 없으므로 제외
	for (const FInventorySlot& Slot : GeneralItems)
	{
		FItemData* Data = GetItemInfo(Slot.ItemID);
		if (!Data) continue;

		// 아이템에 정의된 특수 효과(Proc) 목록 순회
		for (int32 i = 0; i < Data->ProcModifiers.Num(); ++i)
		{
			const FItemProcData& Proc = Data->ProcModifiers[i];

			// 트리거 조건 확인
			if (Proc.TriggerType != Trigger) continue;
			// 쿨타임 확인
			if (Proc.Cooldown > 0.0f && IsOnCooldown(Slot.ItemID, i)) continue;

			// 확률 계산
			// 기본 확률 + (중첩당 추가 확률 * (개수 - 1))
			float FinalChance = Proc.BaseChance + (Proc.StackChance * (Slot.Quantity - 1));

			// 행운 스탯 적용
			//if (CachedStatusComponent)
			//{
			//	FinalChance += CachedStatusComponent->GetResultLuck();
			//}

			// 확률 주사위 굴리기 (0 ~ 100)
			if (FMath::RandRange(0.0f, 100.0f) <= FinalChance)
			{
				//효과 실행
				ExecuteProcEffect(Proc, Slot.Quantity, TargetActor, TriggerValue);

				// 쿨타임이 있는 스킬이라면 쿨타임 등록
				if (Proc.Cooldown > 0.0f)
				{
					SetCooldown(Slot.ItemID, i, Proc.Cooldown);
				}

				UE_LOG(LogTemp, Log, TEXT("Proc Triggered: %s (Item: %s)"), *UEnum::GetValueAsString(Proc.ProcType), *Slot.ItemID.ToString());
			}
		}
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
		// 초기화
	SecretBookSlots.Empty();
	GeneralItems.Empty();
}

bool UInventoryComponent::AddSecretBook(FName BookID, float AddValue)
{
    FItemData* FoundData = GetItemInfo(BookID);
    if (!FoundData)
    {
        UE_LOG(LogTemp, Warning, TEXT("AddSecretBook Failed: Invalid ID [%s]"), *BookID.ToString());
        return false;
    }


    //이미 보유 중이면 레벨업
    for (FInventorySlot& Slot : SecretBookSlots)
    {
        if (Slot.ItemID == BookID)
        {
            Slot.Level++;
            UE_LOG(LogTemp, Log, TEXT("SecretBook Level Up: %s (Lv.%d)"), *BookID.ToString(), Slot.Level);
			ApplyPassiveStats(*FoundData, 1, false, AddValue, EItemType::SecretBook);
			OnItemAdd.Broadcast(BookID, *FoundData);
            return true;
        }
    }

    // 슬롯 제한 확인
    if (SecretBookSlots.Num() >= MaxSecretBookCount)
    {
        UE_LOG(LogTemp, Warning, TEXT("AddSecretBook Failed: Slots Full"));
        return false;
    }

    // 신규 추가
    FInventorySlot NewSlot;
    NewSlot.ItemID = BookID;
    NewSlot.Level = 1;
    NewSlot.Quantity = 1;

    SecretBookSlots.Add(NewSlot);
	ApplyPassiveStats(*FoundData, 1, true, AddValue, EItemType::SecretBook);
    OnItemAdd.Broadcast(BookID, *FoundData);
    UE_LOG(LogTemp, Log, TEXT("SecretBook Added: %s"), *BookID.ToString());

    return true;
}

void UInventoryComponent::AddItem(FName ItemID, int32 Count)
{
    if (Count <= 0) return;

    FItemData* FoundData = GetItemInfo(ItemID);
    if (!FoundData)
    {
        UE_LOG(LogTemp, Warning, TEXT("아이템 추가 실패: Invalid Item ID [%s]"), *ItemID.ToString());
        return;
    }


    // 이미 인벤토리에 있는지 확인 
    for (FInventorySlot& Slot : GeneralItems)
    {
        if (Slot.ItemID == ItemID)
        {
            Slot.Quantity += Count;
            UE_LOG(LogTemp, Log, TEXT("Item 갯수 추가: %s (Qty: %d)"), *ItemID.ToString(), Slot.Quantity);
			ApplyPassiveStats(*FoundData, Count, false, 0.0f, EItemType::GeneralItem);
			
			OnItemAdd.Broadcast(ItemID, *FoundData);
            return;
        }
    }

    // 없으면 새로 추가
    FInventorySlot NewSlot;
    NewSlot.ItemID = ItemID;
    NewSlot.Level = 1; // 일반 아이템은 레벨 의미 없음
    NewSlot.Quantity = Count;

    GeneralItems.Add(NewSlot);
	ApplyPassiveStats(*FoundData, Count, true, 0.0f, EItemType::GeneralItem);
    OnItemAdd.Broadcast(ItemID, *FoundData);
    UE_LOG(LogTemp, Log, TEXT("Item 추가: %s (Qty: %d)"), *ItemID.ToString(), Count);
}

void UInventoryComponent::RemoveItem(FName ItemID, int32 Count)
{
	// 리스트 순회하여 아이템 찾기
	for (int32 i = 0; i < GeneralItems.Num(); ++i)
	{
		if (GeneralItems[i].ItemID == ItemID)
		{
			if (GeneralItems[i].Quantity < Count)
			{
				UE_LOG(LogTemp, Warning, TEXT("제거하려는 수량이 보유량보다 많습니다."));
				return;
			}

			FItemData* FoundData = GetItemInfo(ItemID);
			if (FoundData)
			{
				bool bIsClearing = (GeneralItems[i].Quantity - Count <= 0);

				RemovePassiveStats(*FoundData, Count, bIsClearing, 0.0f, EItemType::GeneralItem);
			}

			// 수량 차감 및 리스트 정리
			GeneralItems[i].Quantity -= Count;

			if (GeneralItems[i].Quantity <= 0)
			{
				GeneralItems.RemoveAt(i);
			}

			if (OnInventoryUpdated.IsBound())
			{
				OnInventoryUpdated.Broadcast();
			}

			UE_LOG(LogTemp, Log, TEXT("Item 제거 완료: %s"), *ItemID.ToString());
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("제거하려는 아이템이 인벤토리에 없습니다."));
	return;
}


FItemData* UInventoryComponent::GetItemInfo(FName ItemID) const
{
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemDataTable is not set in InventoryComponent!"));
		return nullptr;
	}

	static const FString ContextString(TEXT("GetItemInfo"));

	return ItemDataTable->FindRow<FItemData>(ItemID, ContextString);
}

void UInventoryComponent::ApplyPassiveStats(const FItemData& ItemData, int32 Count, bool bIsFirstGet, float AddValue, EItemType ItemType)
{
	ProcessPassiveStats(ItemData, Count, bIsFirstGet, AddValue, ItemType, 1.0f);
}

void UInventoryComponent::RemovePassiveStats(const FItemData& ItemData, int32 Count, bool bIsFirstGet, float AddValue, EItemType ItemType)
{
	ProcessPassiveStats(ItemData, Count, bIsFirstGet, AddValue, ItemType, -1.0f);
}

void UInventoryComponent::ExecuteProcEffect(const FItemProcData& Proc, int32 StackCount, AActor* TargetActor, float TriggerValue)
{
	float TotalValue = Proc.BaseValue + (Proc.StackValue * (StackCount - 1));
	AActor* OwnerActor = GetOwner();

	switch (Proc.ProcType)
	{
	case EProcEffectType::SpawnActor:
		if (!Proc.SpawnActorClass.IsNull())
		{
			UClass* SpawnClass = Proc.SpawnActorClass.LoadSynchronous();
			if (SpawnClass)
			{
				// 소환 위치 결정 (타겟이 있으면 타겟 위치, 없으면 내 위치)
				FVector SpawnLoc = OwnerActor->GetActorLocation() - FVector(0.0f, 0.0f, 75.0f);
				UE_LOG(LogTemp, Log, TEXT("소환"));
				GetWorld()->SpawnActor<AActor>(SpawnClass, SpawnLoc, FRotator::ZeroRotator);
			}
		}
		break;

	case EProcEffectType::FireProjectile:
		// 플레이어의 무기 컴포넌트 등을 통해 발사하거나 직접 스폰
		// (구현된 투사체 로직에 따라 다름)
		break;

	case EProcEffectType::ApplyStatus:
		// 타겟에게 상태이상 부여
		// TargetActor가 있고, 상태이상 인터페이스나 컴포넌트가 있는지 확인 후 적용
		break;

	case EProcEffectType::AreaExplosion:
	{
		FVector Center = (TargetActor) ? TargetActor->GetActorLocation() : OwnerActor->GetActorLocation();
		// 반경 내 데미지
		UGameplayStatics::ApplyRadialDamage(
			this,
			TotalValue, // 데미지
			Center,
			Proc.Radius, // 범위
			UDamageType::StaticClass(),
			TArray<AActor*>(), // 무시할 액터
			OwnerActor 
		);
		// 파티클/사운드 재생 로직 추가
	}
	break;

	case EProcEffectType::ReflectDamage:
		// 반사: 맞았을 때(OnTakeDamage) 들어온 TriggerValue(데미지) 만큼 되돌려줌
		if (TargetActor && TriggerValue > 0.0f)
		{
			// 받은 데미지의 N% 반사 or 고정값 반사
			float ReflectDmg = TriggerValue * (TotalValue / 100.0f);
			UGameplayStatics::ApplyDamage(TargetActor, ReflectDmg, OwnerActor->GetInstigatorController(), OwnerActor, UDamageType::StaticClass());
		}
		break;

	case EProcEffectType::Execute:

	case EProcEffectType::FreeInteract:


	case EProcEffectType::TemporaryBuffStat:
		// StatusComponent에 "일시적 버프"를 거는 함수가 필요함
		// 예: CachedStatusComponent->AddBuff(Proc.StatType, TotalValue, Proc.Duration);
		UE_LOG(LogTemp,Log,TEXT("일시적 버프"));
		break;

	case EProcEffectType::PermanentBuffStat:
	default:
		break;
	}
}

bool UInventoryComponent::IsOnCooldown(FName ItemID, int32 ProcIndex)
{
	FString KeyString = FString::Printf(TEXT("%s_%d"), *ItemID.ToString(), ProcIndex);
	FName Key = FName(*KeyString);

	if (double* EndTime = ProcCooldownMap.Find(Key))
	{
		// 현재 시간이 만료 시간보다 작으면 아직 쿨타임 중
		return GetWorld()->GetTimeSeconds() < *EndTime;
	}
	return false; // 기록 없으면 쿨타임 아님
}

void UInventoryComponent::SetCooldown(FName ItemID, int32 ProcIndex, float CooldownTime)
{
	FString KeyString = FString::Printf(TEXT("%s_%d"), *ItemID.ToString(), ProcIndex);
	FName Key = FName(*KeyString);

	// 언제 쿨타임이 끝나는지 저장
	ProcCooldownMap.Add(Key, GetWorld()->GetTimeSeconds() + CooldownTime);
}

void UInventoryComponent::ProcessPassiveStats(const FItemData& ItemData, int32 Count, bool bIsFirstGet, float AddValue, EItemType ItemType, float Multiplier)
{
	if (!CachedStatusComponent)
	{
		if (AActor* Owner = GetOwner())
		{
			CachedStatusComponent = Owner->FindComponentByClass<UStatusComponent>();
		}
	}
	if (!CachedStatusComponent || Count <= 0) return;

	for (int32 i = 0; i < ItemData.Modifiers.Num(); ++i)
	{
		const FItemStatModifier& Mod = ItemData.Modifiers[i];

		float FinalAddValue = 0.0f;

		// 외부에서 주입된 값(AddValue)이 있다면 최우선 사용
		if (i == 0 && AddValue > 0.0f)
		{
			FinalAddValue = AddValue;
		}
		else
		{
			if (bIsFirstGet)
				FinalAddValue = Mod.BaseValue + (Mod.StackValue * (Count - 1));
			else
				FinalAddValue = Mod.StackValue * Count;
		}

		FinalAddValue *= Multiplier;
		// FinalAddValue를 사용하여 스탯 적용

		auto ApplyStatHelper = [&](TFunction<void(float)> PlayerFunc, TFunction<void(float)> VisionFunc)
			{
				float ApplyVal = FinalAddValue;

				// [A] 비전서(SecretBook)
				// -> StatusComponent 내부 로직에 맡기므로 값 그대로 전달
				if (ItemType == EItemType::SecretBook)
				{
					// 퍼센트 체크 (합연산)
					if (Mod.bIsPercentage)
					{
						ApplyVal = FinalAddValue / 100.0f;
					}
					else
					{
						// 예: 체력 +50 -> 그대로 50 더함
						ApplyVal = FinalAddValue;
					}
					VisionFunc(ApplyVal);
				}
				// [B] 일반 아이템(Player)
				else
				{
					// 퍼센트 체크 (합연산)
					if (Mod.bIsPercentage)
					{
						ApplyVal = FinalAddValue / 100.0f;
					}
					else
					{
						// 예: 체력 +50 -> 그대로 50 더함
						ApplyVal = FinalAddValue;
					}
					PlayerFunc(ApplyVal);
				}

				FString ActionStr = (Multiplier > 0.0f) ? TEXT("적용") : TEXT("제거");
				UE_LOG(LogTemp, Log, TEXT("[%s] %s: %.2f"), *UEnum::GetValueAsString(Mod.StatType), *ActionStr, ApplyVal);
			};

		switch (Mod.StatType)
		{
		case EItemStatType::MaxHP:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerMaxHP(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionMaxHP(Val); });
			break;

		case EItemStatType::HPRegen:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerHPRegen(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionHPRegen(Val); });
			break;

		case EItemStatType::OverHeal:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerOverHeal(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionOverHeal(Val); });
			break;

		case EItemStatType::Shield:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerShield(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionShield(Val); });
			break;

		case EItemStatType::Armor:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerArmor(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionArmor(Val); });
			break;

		case EItemStatType::EvasionChance:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerEvasionChance(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionEvasionChance(Val); });
			break;

		case EItemStatType::LifeDrain:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerLifeDrain(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionLifeDrain(Val); });
			break;

		case EItemStatType::Thorn:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerThorn(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionThorn(Val); });
			break;

		case EItemStatType::ExpGain:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerExpGain(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionExpGain(Val); });
			break;

		case EItemStatType::SilverGain:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerSilverGain(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionSilverGain(Val); });
			break;

		case EItemStatType::GoldGain:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerGoldGain(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionGoldGain(Val); });
			break;

		case EItemStatType::PickUpRange:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerPickUpRange(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionPickUpRange(Val); });
			break;

		case EItemStatType::Difficulty:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerDifficulty(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionDifficulty(Val); });
			break;

		case EItemStatType::Damage:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerDamage(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionDamage(Val); });
			break;

		case EItemStatType::CritChance:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerCriticalChance(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionCriticalChance(Val); });
			break;

			//case EItemStatType::MegaCritChance:
			//	ApplyStatHelper(
			//		[&](float Val) { CachedStatusComponent->AddPlayerMegaCritChance(Val); },
			//		[&](float Val) { CachedStatusComponent->AddVisionMegaCritChance(Val); });
			//	break;

		case EItemStatType::AttackSpeed:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerAttackSpeed(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionAttackSpeed(Val); });
			break;

		case EItemStatType::ProjectileCount:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerProjectileCount(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionProjectileCount(Val); });
			break;

		case EItemStatType::ProjectileReflectCount:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerProjectileReflectCount(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionProjectileReflectCount(Val); });
			break;

		case EItemStatType::AttackSize:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerAttackSize(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionAttackSize(Val); });
			break;

		case EItemStatType::ProjectileSpeed:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerProjectileSpeed(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionProjectileSpeed(Val); });
			break;

		case EItemStatType::AttackDuration:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerAttackDuration(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionAttackDuration(Val); });
			break;

		case EItemStatType::KnockBack:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerKnockBack(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionKnockBack(Val); });
			break;

		case EItemStatType::MoveSpeed:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerMoveSpeed(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionMoveSpeed(Val); });
			break;

		case EItemStatType::JumpPower:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerJumpPower(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionJumpPower(Val); });
			break;

		case EItemStatType::Luck:
			ApplyStatHelper(
				[&](float Val) { CachedStatusComponent->AddPlayerLuck(Val); },
				[&](float Val) { CachedStatusComponent->AddVisionLuck(Val); });
			break;

		default:
			break;
		}
	}
}


