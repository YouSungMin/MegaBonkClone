// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerStatsPanelWidget.h"

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "UI/PlayerStatWidget.h"
#include "Characters/Components/StatusComponent.h"

#include "Internationalization/Text.h"	//FNumberFormattingOptions
#include "UObject/UnrealType.h"        // 리플렉션: FProperty, TFieldIterator, FFloatProperty

void UPlayerStatsPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BuildRows();
}

void UPlayerStatsPanelWidget::NativeDestruct()
{
	if (Status.IsValid())
	{
		Status->OnStatusUpdated.RemoveAll(this);
	}
	Super::NativeDestruct();
}

void UPlayerStatsPanelWidget::BindToStatus(UStatusComponent* InStatus)
{
	if (Status.IsValid())
	{
		Status->OnStatusUpdated.RemoveAll(this);
	}

	Status = InStatus;

	if (Status.IsValid())
	{
		//Status->OnStatusUpdated.AddDynamic(this, &UPlayerStatsPanelWidget::HandleStatusUpdated);
		RefreshValues(); // 최초 1회 갱신
	}
}


void UPlayerStatsPanelWidget::BuildRows()
	{
		if (!StatsBox || !StatRowClass || !StatUITable) return;

		StatsBox->ClearChildren();
		RowWidgets.Reset();
		StatDefs.Reset();

		// 1) 데이터테이블에서 UI 정의 읽기
		TArray<FStatUIRow*> Rows;
		StatUITable->GetAllRows(TEXT("StatUI"), Rows);

		for (FStatUIRow* R : Rows)
		{
			if (R) StatDefs.Add(*R);
		}

		StatDefs.Sort([](const FStatUIRow& A, const FStatUIRow& B) { return A.Order < B.Order; });

		// 2) Row 위젯 생성
		for (const FStatUIRow& Def : StatDefs)
		{
			UPlayerStatWidget* Row = CreateWidget<UPlayerStatWidget>(GetWorld(), StatRowClass);
			if (!Row) continue;

			StatsBox->AddChild(Row);
			Row->SetRowStat(Def.DisplayName, FText::FromString(TEXT("-")));

			RowWidgets.Add(Row);
		}
	}

	void UPlayerStatsPanelWidget::RefreshValues()
	{
		if (!Status.IsValid()) return;

		// RowWidgets <-> StatDefs 인덱스 매칭
		const int32 N = FMath::Min(RowWidgets.Num(), StatDefs.Num());

		for (int32 i = 0; i < N; ++i)
		{
			const FStatUIRow& Def = StatDefs[i];
			UPlayerStatWidget* Row = RowWidgets[i];
			if (!Row) continue;

			FFloatProperty* Prop = FindFProperty<FFloatProperty>(Status->GetClass(), Def.PropertyName);
			if (!Prop)
			{
				Row->SetRowStat(Def.DisplayName, FText::FromString(TEXT("N/A")));
				continue;
			}

			const float Raw = Prop->GetPropertyValue_InContainer(Status.Get());
			Row->SetRowStat(Def.DisplayName, FormatValue(Def, Raw));
		}
	}

	FText UPlayerStatsPanelWidget::FormatValue(const FStatUIRow& Def, float Raw) const
	{
		FNumberFormattingOptions Opt;
		Opt.SetMinimumFractionalDigits(Def.Decimals);
		Opt.SetMaximumFractionalDigits(Def.Decimals);

		auto ToFixed = [&](float V)
			{
				return FText::AsNumber(V, &Opt);
			};

		switch (Def.Format)
		{
		case EStatDisplayFormat::Percent01:
			return FText::FromString(FString::Printf(TEXT("%.*f%%"), Def.Decimals, Raw * 100.f));

		case EStatDisplayFormat::Percent100:
			return FText::FromString(FString::Printf(TEXT("%.*f%%"), Def.Decimals, Raw));

		case EStatDisplayFormat::MultiplierX:
			return FText::FromString(FString::Printf(TEXT("%.*fx"), Def.Decimals, Raw));

		case EStatDisplayFormat::Integer:
			return FText::AsNumber(FMath::RoundToInt(Raw));

		default:
			return ToFixed(Raw);
		}
	}
