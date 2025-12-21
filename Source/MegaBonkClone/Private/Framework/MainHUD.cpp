// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MainHUD.h"
#include "Blueprint/UserWidget.h"
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
}