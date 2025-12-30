// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interactables/MicrowaveActor.h"
#include "MicroShopWidget.generated.h"

class AMicrowaveActor;
class UButton;
class UUniformGridPanel;
class UMicroSlotWidget;
/**
 * 
 */
UCLASS()
class MEGABONKCLONE_API UMicroShopWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
public:
	UFUNCTION(BlueprintCallable)
	void InitWithMicrowave(class AMicrowaveActor* InMicrowave, const TArray<FMicrowaveSlotInfo>& InList);


private:
	//CachedList를 기반으로 그리드 재구성
	void RebuildGrid();

	//슬롯 클릭했을떄 호출되는 함수
	UFUNCTION()
	void OnSlotPicked(FName ItemID);

	//나가기 버튼 클릭했을때 호출되는 함수
	UFUNCTION()
	void OnExitClicked();

private:
	//현재 UI가 연결된 전자레인지 액터
	UPROPERTY()
	TObjectPtr<AMicrowaveActor> MicrowaveRef = nullptr;

	//UI에 표시할 아이템 목록 저장해둠
	UPROPERTY()
	TArray<FMicrowaveSlotInfo> CachedList;

	UFUNCTION()
	void HandleInventoryUpdated();

	// 델리게이트 연결 해제 헬퍼 함수
	void TeardownMicrowave();

protected:
	//슬롯들 배치할 그리드 패널
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> Grid = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton = nullptr;

	// 슬롯 위젯 클래스 (블루프린트에서 지정)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MicroShop")
	TSubclassOf<UMicroSlotWidget> SlotWidgetClass;

	// 한 줄에 몇 칸?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MicroShop")
	int32 Columns = 7;
};
