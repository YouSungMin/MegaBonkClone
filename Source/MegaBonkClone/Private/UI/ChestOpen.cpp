#include "UI/ChestOpen.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
// 1. 초기화
void UChestOpen::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼에 클릭 함수 연결하기
	if (Btn_Claim)
	{
		Btn_Claim->OnClicked.AddDynamic(this, &UChestOpen::OnClaimClicked);

		// 처음엔 버튼 숨기기 (연출 끝나고 보여주기 위해)
		Btn_Claim->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Btn_Decline)
	{
		Btn_Decline->OnClicked.AddDynamic(this, &UChestOpen::OnDeclineClicked);

		// 처음엔 버튼 숨기기 (연출 끝나고 보여주기 위해)
		Btn_Decline->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Btn_OpenChest)
	{
		Btn_OpenChest->OnClicked.AddDynamic(this, &UChestOpen::OnOpenChestClicked);

		// 처음엔 버튼 숨기기 (연출 끝나고 보여주기 위해)
		Btn_OpenChest->SetVisibility(ESlateVisibility::Hidden);
	}

	// 아이템 아이콘도 처음엔 숨김
	if (ItemIcon)
	{
		ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	}
}

// 2. 외부 호출: 연출 시작
void UChestOpen::PlayLootSequence(UTexture2D* NewItemIcon)
{
	// 1) 아이콘 이미지 세팅
	if (ItemIcon && NewItemIcon)
	{
		ItemIcon->SetBrushFromTexture(NewItemIcon);
	}

	// 2) 촬영용 액터에게 "상자 열어" 명령 (3D 애니메이션)
	if (ChestStudioActor)
	{
		// Blueprint에서 만든 PlayOpenAnim 함수 호출
		UE_LOG(LogTemp, Warning, TEXT("PlayOpenAnim"));
		FOutputDeviceNull Ar;
		ChestStudioActor->CallFunctionByNameWithArguments(TEXT("PlayOpenAnim"), Ar, NULL, true);
	}

	// 3) 타이머 설정: 1.2초 뒤에 아이템과 버튼 등장
	// (상자 열리는 애니메이션 시간에 맞춰 조절하세요)
	GetWorld()->GetTimerManager().SetTimer(SequenceTimer, this, &UChestOpen::ShowItemIcon, 1.2f, false);
}

// 3. 타이머 종료: 아이템 및 버튼 표시
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

// 4. 버튼 클릭: 닫기
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
	//PlayLootSequence();

	// 로그 출력
	UE_LOG(LogTemp, Log, TEXT("아이템 열기!"));
}

void UChestOpen::SetStudioActor(AActor* InStudioActor)
{
	ChestStudioActor = InStudioActor;
}