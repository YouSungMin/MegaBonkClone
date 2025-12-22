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
	// [최적화] 비전서는 특수 효과가 없으므로 제외하고, '일반 아이템'만 검사합니다.
	for (const FInventorySlot& Slot : GeneralItems)
	{
		FItemData* Data = GetItemInfo(Slot.ItemID);
		if (!Data) continue;

		// 아이템에 정의된 특수 효과(Proc) 목록 순회
		for (int32 i = 0; i < Data->ProcModifiers.Num(); ++i)
		{
			const FItemProcData& Proc = Data->ProcModifiers[i];

			// 1. 트리거 조건 확인 (예: 맞았을 때인데, 공격 시 효과면 패스)
			if (Proc.TriggerType != Trigger) continue;
			// 2. 쿨타임 확인 (IsOnCooldown 함수는 아까 구현한 것 사용)
			if (Proc.Cooldown > 0.0f && IsOnCooldown(Slot.ItemID, i)) continue;

			// 3. 확률 계산
			// 기본 확률 + (중첩당 추가 확률 * (개수 - 1))
			float FinalChance = Proc.BaseChance + (Proc.StackChance * (Slot.Quantity - 1));

			// (선택 사항) 행운 스탯 적용
			//if (CachedStatusComponent)
			//{
			//	FinalChance += CachedStatusComponent->GetResultLuck();
			//}

			// 4. 확률 주사위 굴리기 (0 ~ 100)
			if (FMath::RandRange(0.0f, 100.0f) <= FinalChance)
			{
				// 발동 성공! -> 효과 실행
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
				// 예: 번개는 타겟 머리 위, 지뢰는 내 발밑 등 기획에 따라 분기 필요
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

		// ... 나머지 케이스 구현 ...
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


