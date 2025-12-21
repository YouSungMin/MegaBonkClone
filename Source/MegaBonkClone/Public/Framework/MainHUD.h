// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/MainHudWidget.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API AMainHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	inline UMainHudWidget* GetMainWidget() const { return MainWidgetInstance; }

protected:
	//mainwidgetclass는 Umainhudwidget를 상속받은 블루프린트 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UMainHudWidget> MainWidgetClass = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UMainHudWidget> MainWidgetInstance = nullptr;	
};