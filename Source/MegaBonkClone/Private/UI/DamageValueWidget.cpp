// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageValueWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "Slate/WidgetTransform.h" 

float UDamageValueWidget::PlayDamagePopup(float Damage)
{
    //텍스트 및 속성 강제 초기화
    if (DamageText)
    {
        DamageText->SetText(FText::AsNumber(FMath::RoundToInt(Damage)));
        // 텍스트 블록 자체에 애니메이션이 걸려있다면 초기화
        DamageText->ForceLayoutPrepass();

        DamageText->SetRenderOpacity(1.0f);
        DamageText->SetRenderTransform(FWidgetTransform());
    }

    //위젯 초기화
    SetRenderOpacity(1.0f);
    SetRenderTransform(FWidgetTransform());

    //애니메이션 재생
    if (DamagePopupAnimation)
    {
        StopAnimation(DamagePopupAnimation); // 확실히 멈춤

        // 0.0초부터, 1회 재생, 정방향, 1배속
        PlayAnimation(DamagePopupAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);

        return DamagePopupAnimation->GetEndTime();
    }

    return 1.0f;
}