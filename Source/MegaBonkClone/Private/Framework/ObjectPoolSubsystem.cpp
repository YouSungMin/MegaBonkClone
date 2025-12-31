// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/ObjectPoolSubsystem.h"
#include "Interfaces/ObjectPoolInterface.h"
#include "MegaBonkClone/MegaBonkClone.h"

AActor* UObjectPoolSubsystem::SpawnPooledActor(UClass* Class, FVector location, 
	FRotator rotation, AActor* Owner, APawn* Instigator)
{
	if (!Class) return nullptr;

	FObjectPoolQueue& PoolQueue = PoolMap.FindOrAdd(Class);

	AActor* PooledActor = nullptr;

	//풀에 남는 게 있는지 확인 (유효하지 않은 건 버림)
	while (PoolQueue.Pool.Num() > 0)
	{
		AActor* Candidate = PoolQueue.Pool.Pop();
		if (IsValid(Candidate))
		{
			PooledActor = Candidate;
			UE_LOG(LogObjectPool, Warning, TEXT("♻️ [ObjectPool] 재사용 성공 (Reuse): %s (남은 개수: %d)"), 
				*PooledActor->GetName(), PoolQueue.Pool.Num());
			break;
		}
	}

	//풀에 없으면 새로 생성
	if (!PooledActor)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		PooledActor = GetWorld()->SpawnActor<AActor>(Class, location, rotation, Params);

		if (PooledActor)
		{
			UE_LOG(LogObjectPool, Warning, TEXT("✨ [ObjectPool] 신규 생성 (New Spawn): %s"), 
				*PooledActor->GetName());
		}
	}
	else
	{
		// 풀에서 꺼냈으면 위치/회전 강제 지정
		PooledActor->SetActorLocationAndRotation(location, rotation);
	}

	//활성화 처리 (인터페이스 호출)
	if (PooledActor && PooledActor->Implements<UObjectPoolInterface>())
	{ 
		//(초기화)
		IObjectPoolInterface::Execute_OnPoolActivate(PooledActor);
	}
	return PooledActor;
}

void UObjectPoolSubsystem::ReturnToPool(AActor* InActor)
{
	if (IsValid(InActor)) {
		if (InActor->Implements<UObjectPoolInterface>()) {
			IObjectPoolInterface::Execute_OnPoolDeactivate(InActor);
		}
		else {
			InActor->Destroy();
			UE_LOG(LogObjectPool, Error, TEXT("IObjectPoolInterface : 인터페이스 구현안되있음"));
			return;
		}

		FObjectPoolQueue& poolQueue = PoolMap.FindOrAdd(InActor->GetClass());
		poolQueue.Pool.Push(InActor);

		UE_LOG(LogObjectPool, Warning, TEXT("📥 [ObjectPool] 반납 완료 (Return): %s (현재 보유량: %d)"), 
			*InActor->GetName(), poolQueue.Pool.Num());
	}
	
}
