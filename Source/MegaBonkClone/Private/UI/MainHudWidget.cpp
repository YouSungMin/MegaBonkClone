// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHudWidget.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Characters/Components/StatusComponent.h"
#include "UI/ResourceBarWidget.h"


void UMainHudWidget::NativeConstruct()
{
	//Super::NativeConstruct();

	//APlayerCharacter* player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	//
	//if(player && player->GetStatusComponent())
	//{
	//	//UStatusComponent* resource = player->GetStatusComponent();
	//	////HealthBar->OnHealthChanged.Broadcast(resource->GetCurrentHP(), resource->GetMaxHP());
	//	//
	//	//resource->OnHealthChanged.AddDynamic(HealthBar.Get(), &UResourceBarWidget::RefreshWidget);
	//	//HealthBar->RefreshWidget(resource->GetCurrentHP(), resource->GetMaxHP());
	//	///*ShieldBar->RefreshWidget(resource->)*/
}
