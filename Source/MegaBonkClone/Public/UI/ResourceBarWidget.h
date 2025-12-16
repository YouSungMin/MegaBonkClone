// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourceBarWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, Current, float, Max);
UCLASS()
class MEGABONKCLONE_API UResourceBarWidget : public UUserWidget
{
	GENERATED_BODY()

public: 
	//HP변화 알람용 델리게이트
	UPROPERTY(BlueprintAssignable, Category="Resource")
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable)
	void RefreshWidget(float InCurrent, float InMax);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	virtual void NativePreConstruct() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TWeakObjectPtr<class UProgressBar> Bar; //프로그래스바

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TWeakObjectPtr<class UTextBlock> Current; //현재 퍼센트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TWeakObjectPtr<class UTextBlock> Max; //최대 퍼센트


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Color")
	FLinearColor FillColor = FLinearColor(0.0f, 1.0f, 0.0f); //바 퍼센트 컬러

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Color")
	FLinearColor BackgroundColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.2f); //바 뒷배경 컬러
};
