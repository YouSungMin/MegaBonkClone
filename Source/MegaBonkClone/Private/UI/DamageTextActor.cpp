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
	SetActorHiddenInGame(false);

	//위젯 컴포넌트가 렌더링 준비를 마칠 시간을 주기 위해 '다음 틱'에 실행합니다.
	// 람다 함수를 사용하여 안전하게 처리합니다.

	TWeakObjectPtr<ADamageTextActor> WeakThis = this;

	GetWorld()->GetTimerManager().SetTimerForNextTick([WeakThis, DamageAmount]()
	{
		// 액터가 소멸되지 않았는지 확인
		if (!WeakThis.IsValid()) return;

		ADamageTextActor* Self = WeakThis.Get();

		// 위젯 가져오기
		if (UDamageValueWidget* Widget = Cast<UDamageValueWidget>(Self->DamageWidgetComp->GetUserWidgetObject()))
		{
			// 여기서 애니메이션 재생 (초기화 및 Play)
			float AnimDuration = Widget->PlayDamagePopup(DamageAmount);

			//애니메이션 끝나는 시간에 맞춰 반납 타이머 설정
			Self->GetWorld()->GetTimerManager().SetTimer(Self->DestroyTimerHandle, Self, &ADamageTextActor::ReturnToPool, AnimDuration, false);
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