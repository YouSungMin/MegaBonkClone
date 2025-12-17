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
	//HP��ȭ �˶��� ��������Ʈ
	UPROPERTY(BlueprintAssignable, Category="Resource")
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable)
	void RefreshWidget(float InCurrent, float InMax);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TWeakObjectPtr<class UProgressBar> Bar; //���α׷�����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TWeakObjectPtr<class UTextBlock> Current; //���� �ۼ�Ʈ

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TWeakObjectPtr<class UTextBlock> Max; //�ִ� �ۼ�Ʈ

};
