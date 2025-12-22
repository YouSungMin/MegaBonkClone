// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChestOpen.generated.h"

// 전방 선언
class UImage;
class UButton;

UCLASS()
class MEGABONKCLONE_API UChestOpen : public UUserWidget
{
	GENERATED_BODY()

public:
	// 외부(ChestActor)에서 이 함수를 호출해서 연출을 시작합니다.
	UFUNCTION(BlueprintCallable)
	void PlayLootSequence(UTexture2D* NewItemIcon);

	// 촬영용 스튜디오 액터를 설정하는 함수
	UFUNCTION(BlueprintCallable)
	void SetStudioActor(AActor* InStudioActor);

protected:
	// 위젯이 생성될 때 실행되는 함수 (초기화)
	virtual void NativeConstruct() override;

	// 버튼 클릭 시 실행될 함수
	UFUNCTION()
	void OnClaimClicked();

	UFUNCTION()
	void OnDeclineClicked();

	UFUNCTION()
	void OnOpenChestClicked();
	// 타이머가 끝나면 아이콘을 보여주는 함수
	void ShowItemIcon();

protected:
	// [UI 바인딩] 에디터의 위젯 디자이너에 있는 이름과 똑같아야 합니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ChestOpen", meta = (BindWidget))
	TObjectPtr<UImage> Chest3D;   // 3D 상자 화면 (Render Target)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ChestOpen", meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;  // 2D 아이템 아이콘

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ChestOpen", meta = (BindWidget))
	TObjectPtr<UButton> Btn_OpenChest;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ChestOpen", meta = (BindWidget))
	TObjectPtr<UButton> Btn_Claim;  
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ChestOpen", meta = (BindWidget))
	TObjectPtr<UButton> Btn_Decline;

	// [애니메이션 바인딩] (선택 사항)
	//UPROPERTY(meta = (BindWidgetAnim), Transient)
	//TObjectPtr<UWidgetAnimation> Anim_IconAppear;

private:
	// 촬영용 액터 참조
	TObjectPtr<AActor> ChestStudioActor;

	// 타이머 핸들
	FTimerHandle SequenceTimer;
};