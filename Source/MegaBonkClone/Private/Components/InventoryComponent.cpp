// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Characters/Components/StatusComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


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

bool UInventoryComponent::AddSecretBook(FName BookID)
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
			ApplyPassiveStats(*FoundData, 1, false);
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
	ApplyPassiveStats(*FoundData, 1, true);
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
			ApplyPassiveStats(*FoundData, Count, false);
			
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
	ApplyPassiveStats(*FoundData, Count, true);
    OnItemAdd.Broadcast(ItemID, *FoundData);
    UE_LOG(LogTemp, Log, TEXT("Item 추가: %s (Qty: %d)"), *ItemID.ToString(), Count);
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

void UInventoryComponent::ApplyPassiveStats(const FItemData& ItemData, int32 Count, bool bIsFirstGet)
{
	if (!CachedStatusComponent)
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			CachedStatusComponent = Owner->FindComponentByClass<UStatusComponent>();
		}
	}
    if (!CachedStatusComponent || Count <= 0) return;

    for (const FItemStatModifier& Mod : ItemData.Modifiers)
    {
        float RawValue = 0.0f;
        if (bIsFirstGet)
            RawValue = Mod.BaseValue + (Mod.StackValue * (Count - 1));
        else
            RawValue = Mod.StackValue * Count;

        auto ApplyStatHelper = [&](float CurrentBaseStat, TFunction<void(float)> SetterFunc)
            {
                float FinalAddValue = RawValue;

                // 퍼센트 체크 로직을 여기서 한 번만 작성하면 됩니다!
                if (Mod.bIsPercentage)
                {
                    FinalAddValue = CurrentBaseStat * (RawValue / 100.0f);
                }

                // 실제 적용 함수 실행
                SetterFunc(FinalAddValue);

                // 로그 출력 (선택 사항)
                UE_LOG(LogTemp, Log, TEXT("Stat Applied [%s]: Added %.2f"), *UEnum::GetValueAsString(Mod.StatType), FinalAddValue);
            };

        switch (Mod.StatType)
        {
		case EItemStatType::MaxHP:
			ApplyStatHelper(CachedStatusComponent->GetResultMaxHP(), [&](float Val) {
				CachedStatusComponent->AddPlayerMaxHP(Val);
				});
			break;

		case EItemStatType::HPRegen:
			ApplyStatHelper(CachedStatusComponent->GetResultHPRegen(), [&](float Val) {
				CachedStatusComponent->AddPlayerHPRegen(Val);
				});
			break;

		case EItemStatType::OverHeal:
			ApplyStatHelper(CachedStatusComponent->GetResultOverHeal(), [&](float Val) {
				CachedStatusComponent->AddPlayerOverHeal(Val);
				});
			break;

		case EItemStatType::Shield:
			ApplyStatHelper(CachedStatusComponent->GetResultShield(), [&](float Val) {
				CachedStatusComponent->AddPlayerShield(Val);
				});
			break;

		case EItemStatType::Armor:
			ApplyStatHelper(CachedStatusComponent->GetResultArmor(), [&](float Val) {
				CachedStatusComponent->AddPlayerArmor(Val);
				});
			break;

		case EItemStatType::EvasionChance:
			ApplyStatHelper(CachedStatusComponent->GetResultEvasionChance(), [&](float Val) {
				CachedStatusComponent->AddPlayerEvasionChance(Val);
				});
			break;

		case EItemStatType::LifeDrain:
			ApplyStatHelper(CachedStatusComponent->GetResultLifeDrain(), [&](float Val) {
				CachedStatusComponent->AddPlayerLifeDrain(Val);
				});
			break;

		case EItemStatType::Thorn:
			ApplyStatHelper(CachedStatusComponent->GetResultThorn(), [&](float Val) {
				CachedStatusComponent->AddPlayerThorn(Val);
				});
			break;

		case EItemStatType::ExpGain:
			ApplyStatHelper(CachedStatusComponent->GetResultExpGain(), [&](float Val) {
				CachedStatusComponent->AddPlayerExpGain(Val);
				});
			break;

		case EItemStatType::SilverGain:
			ApplyStatHelper(CachedStatusComponent->GetResultSilverGain(), [&](float Val) {
				CachedStatusComponent->AddPlayerSilverGain(Val);
				});
			break;

		case EItemStatType::GoldGain:
			ApplyStatHelper(CachedStatusComponent->GetResultGoldGain(), [&](float Val) {
				CachedStatusComponent->AddPlayerGoldGain(Val);
				});
			break;

		case EItemStatType::PickUpRange:
			ApplyStatHelper(CachedStatusComponent->GetResultPickUpRange(), [&](float Val) {
				CachedStatusComponent->AddPlayerPickUpRange(Val);
				});
			break;

		case EItemStatType::Difficulty:
			ApplyStatHelper(CachedStatusComponent->GetResultDifficulty(), [&](float Val) {
				CachedStatusComponent->AddPlayerDifficulty(Val);
				});
			break;

		case EItemStatType::Damage:
			ApplyStatHelper(CachedStatusComponent->GetResultDamage(), [&](float Val) {
				CachedStatusComponent->AddPlayerDamage(Val);
				});
			break;

		case EItemStatType::CritChance:
			ApplyStatHelper(CachedStatusComponent->GetResultCriticalChance(), [&](float Val) {
				CachedStatusComponent->AddPlayerCriticalChance(Val);
				});
			break;

		//case EItemStatType::MegaCritChance:
		//	ApplyStatHelper(CachedStatusComponent->GetResultMegaCritChance(), [&](float Val) {
		//		CachedStatusComponent->AddPlayerMegaCritChance(Val);
		//		});
		//	break;

		case EItemStatType::AttackSpeed:
			ApplyStatHelper(CachedStatusComponent->GetResultAttackSpeed(), [&](float Val) {
				CachedStatusComponent->AddPlayerAttackSpeed(Val);
				});
			break;

		case EItemStatType::ProjectileCount:
			ApplyStatHelper(CachedStatusComponent->GetResultProjectileCount(), [&](float Val) {
				CachedStatusComponent->AddPlayerProjectileCount(Val);
				});
			break;

		case EItemStatType::ProjectileReflectCount:
			ApplyStatHelper(CachedStatusComponent->GetResultProjectileReflectCount(), [&](float Val) {
				CachedStatusComponent->AddPlayerProjectileReflectCount(Val);
				});
			break;

		case EItemStatType::AttackSize:
			ApplyStatHelper(CachedStatusComponent->GetResultAttackSize(), [&](float Val) {
				CachedStatusComponent->AddPlayerAttackSize(Val);
				});
			break;

		case EItemStatType::ProjectileSpeed:
			ApplyStatHelper(CachedStatusComponent->GetResultProjectileSpeed(), [&](float Val) {
				CachedStatusComponent->AddPlayerProjectileSpeed(Val);
				});
			break;

		case EItemStatType::AttackDuration:
			ApplyStatHelper(CachedStatusComponent->GetResultAttackDuration(), [&](float Val) {
				CachedStatusComponent->AddPlayerAttackDuration(Val);
				});
			break;

		case EItemStatType::KnockBack:
			ApplyStatHelper(CachedStatusComponent->GetResultKnockBack(), [&](float Val) {
				CachedStatusComponent->AddPlayerKnockBack(Val);
				});
			break;

		case EItemStatType::MoveSpeed:
			ApplyStatHelper(1.0f, [&](float Val) {
				CachedStatusComponent->AddPlayerMoveSpeed(Val);
				});
			break;

		case EItemStatType::JumpPower:
			ApplyStatHelper(CachedStatusComponent->GetResultJumpPower(), [&](float Val) {
				CachedStatusComponent->AddPlayerJumpPower(Val);
				});
			break;

		case EItemStatType::Luck:
			ApplyStatHelper(CachedStatusComponent->GetResultLuck(), [&](float Val) {
				CachedStatusComponent->AddPlayerLuck(Val);
				});
			break;
        default:
            break;
        }
    }
}



