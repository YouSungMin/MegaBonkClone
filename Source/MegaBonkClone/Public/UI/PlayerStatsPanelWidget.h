// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "PlayerStatsPanelWidget.generated.h"

class UVerticalBox;
class UStatusComponent;
class UPlayerStatWidget;

UENUM(BlueprintType)
enum class EStatDisplayFormat : uint8
{
	Number,
	Percent01,     // 0~1 값을 0~100%로 (Armor/Evasion 같은 거)
	Percent100,    // 이미 0~100인 값에 % 붙이기 (CritChance 같은 거)
	MultiplierX,   // 1.23x 형태 (Damage 같은 거)
	Integer
};

USTRUCT(BlueprintType)
struct FStatUIRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName PropertyName; // 예: "ResultMaxHP"

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;  // 예: "최대 HP"

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EStatDisplayFormat Format = EStatDisplayFormat::Number;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Decimals = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Order = 0;
};

/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UPlayerStatsPanelWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
void BindToStatus(UStatusComponent* InStatus);

protected:
	virtual void NativeConstruct() override; 
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> StatsBox = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPlayerStatWidget> StatRowClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UDataTable> StatUITable = nullptr;


private:
	UFUNCTION()
	void HandleStatusUpdated();

	void BuildRows();
	void RefreshValues();

	FText FormatValue(const FStatUIRow& Def, float Raw) const;

private:
	TWeakObjectPtr<UStatusComponent> Status;
	TArray<FStatUIRow> StatDefs;
	TArray<TObjectPtr<UPlayerStatWidget>> RowWidgets;
};