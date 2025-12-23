#include "UI/ChestOpen.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Data/ItemDataStructs.h"
#include "Misc/OutputDeviceNull.h"
//초기화
void UChestOpen::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼에 클릭 함수 연결하기
	if (Btn_Claim)
	{
		Btn_Claim->OnClicked.AddDynamic(this, &UChestOpen::OnClaimClicked);
	}

	if (Btn_Decline)
	{
		Btn_Decline->OnClicked.AddDynamic(this, &UChestOpen::OnDeclineClicked);
	}

	if (Btn_OpenChest)
	{
		Btn_OpenChest->OnClicked.AddDynamic(this, &UChestOpen::OnOpenChestClicked);;
	}


}

//외부 호출: 연출 시작
void UChestOpen::PlayLootSequence(const FItemData& InRowData)
{
	//열기 버튼만 보이기
	if (Btn_OpenChest) Btn_OpenChest->SetVisibility(ESlateVisibility::Visible); 
	//이아래는 숨기기
	if (Btn_Claim) Btn_Claim->SetVisibility(ESlateVisibility::Hidden);         
	if (Btn_Decline) Btn_Decline->SetVisibility(ESlateVisibility::Hidden);      
	if (ItemIcon) ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	if (ItemName) ItemName->SetVisibility(ESlateVisibility::Hidden);
	if (ItemDescription) ItemDescription->SetVisibility(ESlateVisibility::Hidden);
	if (ItemGrade) ItemGrade->SetVisibility(ESlateVisibility::Hidden);

	//아이템 정보 세팅
	//아이템 이름
	if (ItemName)
	{
		ItemName->SetText(InRowData.Name);
	}

	//아이템 설명
	if (ItemDescription)
	{
		ItemDescription->SetText(InRowData.Description);
	}

	//아이템 등급 (텍스트 + 색상 처리)
	if (ItemGrade)
	{
		FText GradeText;
		FSlateColor GradeColor = FSlateColor(FLinearColor::White); // 기본 흰색

		// 등급에 따라 텍스트와 색상을 다르게 설정 (스위치문 활용)
		switch (InRowData.Grade)
		{
		case EItemGrade::Common:
			GradeText = FText::FromString(TEXT("Common"));
			GradeColor = FSlateColor(FLinearColor(0.8f, 0.8f, 0.8f, 1.0f)); // 회색
			break;
		case EItemGrade::UnCommon:
			GradeText = FText::FromString(TEXT("UnCommon"));
			GradeColor = FSlateColor(FLinearColor(0.1f, 1.0f, 0.1f, 1.0f));
			break;
		case EItemGrade::Rare:
			GradeText = FText::FromString(TEXT("Rare"));
			GradeColor = FSlateColor(FLinearColor(0.0f, 0.5f, 1.0f, 1.0f)); // 파랑
			break;
		case EItemGrade::Epic:
			GradeText = FText::FromString(TEXT("Epic"));
			GradeColor = FSlateColor(FLinearColor(0.6f, 0.0f, 1.0f, 1.0f)); // 보라
			break;
		case EItemGrade::Legendary:
			GradeText = FText::FromString(TEXT("Legendary"));
			GradeColor = FSlateColor(FLinearColor(1.0f, 0.5f, 0.0f, 1.0f)); // 주황
			break;
		default:
			GradeText = FText::FromString(TEXT("Unknown"));
			break;
		}

		ItemGrade->SetText(GradeText);
		ItemGrade->SetColorAndOpacity(GradeColor);
	}

	UTexture2D* LoadedTexture = InRowData.Icon.LoadSynchronous();

	if (ItemIcon && LoadedTexture)
	{
		// 텍스처 설정
		ItemIcon->SetBrushFromTexture(LoadedTexture);
	}

	if (ChestStudioActor)
	{
		FOutputDeviceNull Ar;
		// BP에 만든 'StartIntroSequence' 이벤트 호출
		ChestStudioActor->CallFunctionByNameWithArguments(TEXT("StartIntroSequence"), Ar, NULL, true);
	}

}

//타이머 종료: 아이템 및 버튼 표시
void UChestOpen::ShowItemIcon()
{
	if (ItemIcon)
	{
		ItemIcon->SetVisibility(ESlateVisibility::Visible);

		// 등장 애니메이션 재생 (있을 경우)
		/*if (Anim_IconAppear)
		{
			PlayAnimation(Anim_IconAppear);
		}*/
	}

	// 확인 버튼도 이제 보여줌
	if (Btn_Claim)
	{
		Btn_Claim->SetVisibility(ESlateVisibility::Visible);
	}
	if (Btn_Decline)
	{
		Btn_Decline->SetVisibility(ESlateVisibility::Visible);
	}
}

//버튼 클릭: 닫기
void UChestOpen::OnClaimClicked()
{
	// 위젯 닫기
	RemoveFromParent();

	// 로그 출력
	UE_LOG(LogTemp, Log, TEXT("아이템 획득 확인! 창을 닫습니다."));
}

void UChestOpen::OnDeclineClicked()
{
	// 위젯 닫기
	RemoveFromParent();

	// 로그 출력
	UE_LOG(LogTemp, Log, TEXT("아이템 획득 거절! 창을 닫습니다."));
}

void UChestOpen::OnOpenChestClicked()
{
	
	if (Btn_OpenChest)Btn_OpenChest->SetVisibility(ESlateVisibility::Hidden);
	if (ItemName) ItemName->SetVisibility(ESlateVisibility::Visible);
	if (ItemDescription) ItemDescription->SetVisibility(ESlateVisibility::Visible);
	if (ItemGrade) ItemGrade->SetVisibility(ESlateVisibility::Visible);

	if (ChestStudioActor)
	{
		FOutputDeviceNull Ar;
		// BP에 만든 'StartOpenSequence' 이벤트 호출
		ChestStudioActor->CallFunctionByNameWithArguments(TEXT("StartOpenSequence"), Ar, NULL, true);
	}

	
	float OpenAnimDuration = 1.2f;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(SequenceTimer, this, &UChestOpen::ShowItemIcon, OpenAnimDuration, false);
	}

	UE_LOG(LogTemp, Log, TEXT("상자 열기 시작!"));
}

void UChestOpen::SetStudioActor(AActor* InStudioActor)
{
	ChestStudioActor = InStudioActor;
}