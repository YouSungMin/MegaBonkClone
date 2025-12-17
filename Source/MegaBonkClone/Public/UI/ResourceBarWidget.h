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
	//HPï¿½ï¿½È­ ï¿½Ë¶ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ®
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
	TWeakObjectPtr<class UProgressBar> Bar; //ï¿½ï¿½ï¿½Î±×·ï¿½ï¿½ï¿½ï¿½ï¿½

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TWeakObjectPtr<class UTextBlock> Current; //ï¿½ï¿½ï¿½ï¿½ ï¿½Û¼ï¿½Æ®

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TWeakObjectPtr<class UTextBlock> Max; //ï¿½Ö´ï¿½ ï¿½Û¼ï¿½Æ®


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Color")
	FLinearColor FillColor = FLinearColor(0.0f, 1.0f, 0.0f); //¹Ù ÆÛ¼¾Æ® ÄÃ·¯

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Color")
	FLinearColor BackgroundColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.2f); //¹Ù µÞ¹è°æ ÄÃ·¯
};
