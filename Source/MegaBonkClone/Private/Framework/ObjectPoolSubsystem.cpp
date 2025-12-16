// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/ObjectPoolSubsystem.h"
#include "Interfaces/ObjectPoolInterface.h"

AActor* UObjectPoolSubsystem::SpawnPooledActor(UClass* Class, FVector location, FRotator rotation)
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
			break;
		}
	}

	//풀에 없으면 새로 생성
	if (!PooledActor)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		PooledActor = GetWorld()->SpawnActor<AActor>(Class, location, rotation, Params);
	}
	else
	{
		// 풀에서 꺼냈으면 위치/회전 강제 지정
		PooledActor->SetActorLocationAndRotation(location, rotation);
	}

	//활성화 처리 (인터페이스 호출)
	if (PooledActor && PooledActor->Implements<UObjectPoolInterface>())
	{
		// 꺼내짐 알림 (초기화)
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
			UE_LOG(LogTemp, Error, TEXT("IObjectPoolInterface : 인터페이스 구현안되있음"));
			return;
		}

		FObjectPoolQueue& poolQueue = PoolMap.FindOrAdd(InActor->GetClass());
		poolQueue.Pool.Push(InActor);
	}
	
}
