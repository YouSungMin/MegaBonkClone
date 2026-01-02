// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DamageTextActor.h"
#include "UI/DamageValueWidget.h" 
#include "Components/WidgetComponent.h"
#include "Framework/ObjectPoolSubsystem.h"
#include "TimerManager.h"

ADamageTextActor::ADamageTextActor()
{
	PrimaryActorTick.bCanEverTick = false; // 움직임 계산이 없으므로 틱 꺼도 됨

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// 위젯 컴포넌트 설정
	DamageWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidgetComp"));
	DamageWidgetComp->SetupAttachment(Root);
	DamageWidgetComp->SetWidgetSpace(EWidgetSpace::Screen); // 스크린 모드
	DamageWidgetComp->SetDrawAtDesiredSize(true);
	DamageWidgetComp->SetCollisionProfileName(TEXT("NoCollision"));

}

void ADamageTextActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADamageTextActor::SetDamageValue(float DamageAmount)
{
	SetActorHiddenInGame(true);

	TWeakObjectPtr<ADamageTextActor> WeakThis = this;

	GetWorld()->GetTimerManager().SetTimerForNextTick([WeakThis, DamageAmount]()
		{
			// 액터 유효성 검사
			if (!WeakThis.IsValid()) return;

			ADamageTextActor* Self = WeakThis.Get();

			// 위젯 가져오기
			if (UDamageValueWidget* Widget = Cast<UDamageValueWidget>(Self->DamageWidgetComp->GetUserWidgetObject()))
			{
				// 1. 먼저 값을 세팅하고 애니메이션 준비
				float AnimDuration = Widget->PlayDamagePopup(DamageAmount);

				// 2. [핵심] 값이 바뀐 것이 확정된 후 보이게 설정
				Self->SetActorHiddenInGame(false);

				// 3. 반납 타이머 설정
				Self->GetWorld()->GetTimerManager().SetTimer(Self->DestroyTimerHandle, Self, &ADamageTextActor::ReturnToPool, AnimDuration, false);
			}
			else
			{
				// 혹시 위젯을 못 찾았으면 그냥 즉시 반납 (안전장치)
				Self->ReturnToPool();
			}
		});
}

void ADamageTextActor::OnPoolActivate_Implementation()
{
	// 활성화될 때는 아직 숨겨둠 (SetDamageValue에서 켬)
	// 이전 타이머가 있다면 초기화
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
}

void ADamageTextActor::OnPoolDeactivate_Implementation()
{
	// 반납 시 숨기기
	SetActorHiddenInGame(true);

	// 실행 중인 타이머 강제 종료
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
}

void ADamageTextActor::ReturnToPool()
{
	// 타이머 종료 시 호출되어 풀로 반납
	UWorld* World = GetWorld();
	if (World)
	{
		if (auto* Pool = World->GetSubsystem<UObjectPoolSubsystem>())
		{
			Pool->ReturnToPool(this);
			return;
		}
	}
	Destroy(); // 풀 시스템이 없으면 그냥 파괴
}