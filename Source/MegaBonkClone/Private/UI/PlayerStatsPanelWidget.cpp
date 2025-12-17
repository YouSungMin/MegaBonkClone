// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerStatsPanelWidget.h"

#include "Components/VerticalBox.h"
#include "UI/PlayerStatWidget.h"
#include "Characters/Components/StatusComponent.h"

#include "UObject/UnrealType.h"        // 리플렉션: FProperty, TFieldIterator, FFloatProperty



void UPlayerStatsPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerStatsPanelWidget::InitWithStatus(UStatusComponent* InStatusComp)
{
	StatusComp = InStatusComp;

	BuildRows_Auto();
}

void UPlayerStatsPanelWidget::BuildRows_Auto()
{
	if (RowByProp.Num() > 0) return;

	StatBox->ClearChildren(); //기존UI비우기
	RowByProp.Empty();	//맵 초기화

	//StatusComponent 클래스 UPROPERTY 순회
	for (TFieldIterator<FProperty> It(StatusComp->GetClass()); It; ++It)
	{
		FProperty* Prop = *It;	//현재 변수정보

		if (!Prop->HasMetaData(TEXT("StatUI")))	//StatusComponent 변수에 meta=(StatUI) 붙었을때
			continue;

		FFloatProperty* FloatProp = CastField<FFloatProperty>(Prop);
		if (!FloatProp)
			continue;

		//값 읽기
		const float Value = FloatProp->GetPropertyValue_InContainer(StatusComp);

		//이름 읽기
		const FText Label = Prop->GetDisplayNameText();

		UPlayerStatWidget* Row = CreateWidget<UPlayerStatWidget>
	}


}
