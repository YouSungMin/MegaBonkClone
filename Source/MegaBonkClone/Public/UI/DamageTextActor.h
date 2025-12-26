// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ObjectPoolInterface.h" 
#include "DamageTextActor.generated.h"

UCLASS()
class MEGABONKCLONE_API ADamageTextActor : public AActor, public IObjectPoolInterface
{
	GENERATED_BODY()

public:
	ADamageTextActor();

protected:
	virtual void BeginPlay() override;

public:
	// 외부(Enemy)에서 호출할 함수
	void SetDamageValue(float DamageAmount);

	//오브젝트 풀 인터페이스 구현부
	virtual void OnPoolActivate_Implementation() override;
	virtual void OnPoolDeactivate_Implementation() override;

protected:
	// 위젯 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UWidgetComponent> DamageWidgetComp;

private:
	// 수명 관리용 타이머 핸들
	FTimerHandle DestroyTimerHandle;

	// 타이머가 끝나면 호출될 함수
	UFUNCTION()
	void ReturnToPool();
};