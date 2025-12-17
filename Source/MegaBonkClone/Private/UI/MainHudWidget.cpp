// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHudWidget.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Characters/Components/StatusComponent.h"
#include "UI/ResourceBarWidget.h"


void UMainHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter* player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	
	if(player && player->GetStatusComponent())
	{
		//UStatusComponent* resource = player->GetStatusComponent();

		if(HealthBar.IsValid())
		{
		HealthBar->RefreshWidget(resource->GetCurrentHP(), resource->GetResultMaxHP());
		}
		//ShieldBar->RefreshWidget(resource->)

	}
}
