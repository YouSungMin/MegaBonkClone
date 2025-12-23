// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MainHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PlayAbleCharacter/PlayAbleCharacterController.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();
	if (MainWidgetClass)
	{
		MainWidgetInstance = CreateWidget<UMainHudWidget>(GetWorld(), MainWidgetClass);

		if (MainWidgetInstance)
		{
			MainWidgetInstance->AddToViewport();

			//HUD는 컨트롤러가 소유하므로, 컨트롤러에 메인위젯을 설정
			APlayAbleCharacterController* pc = Cast<APlayAbleCharacterController>(GetOwningPlayerController());
			if (pc)
			{
				pc->InitializeMainHudWidget(MainWidgetInstance);
			}
		}
	}

	TArray<AActor*> FoundActors;

	// 모~든 액터를 다 가져오는 건 여전히 무겁지만, BeginPlay는 로딩 때 한 번만 하니까 괜찮습니다.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		// 이름으로 찾기 (혹은 태그로 찾는 게 더 안전합니다)
		if (Actor && Actor->ActorHasTag("ChestStudio"))
		{
			CachedStudioActor = Actor;
			UE_LOG(LogTemp, Log, TEXT("촬영용 스튜디오 액터 찾음: %s"), *Actor->GetName());
			break; 
		}
	}
}