// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UPlayerStatsPanelWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override; 

public:
	UFUNCTION(BlueprintCallable, Category = "Stat")
	void InitWithStatus(class UStatusComponent* InStatusComp);	//StatusComponent 넣어주기

private: 
	void BuildRows_Auto();	//StatusComponent안 변수들 Row에 생성해 화면에 자동나열 

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> StatBox;		//Row들어갈 박스

	UPROPERTY(EditAnywhere, Category = "Stat")
	TSubclassOf<class UPlayerStatWidget> PlayerStatRowClass;	//Row 클래스 -> WBP_PlayerStat넣어주기

private:
	UPROPERTY()
	TObjectPtr<class UStatusComponent> StatusComp;	//플레이어가 가진 

	UPROPERTY()
	TMap<FName, TObjectPtr<class UPlayerStatWidget>> RowByProp;
};
