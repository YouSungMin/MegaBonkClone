// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Interfaces/PickupInterface.h"
#include "Interfaces/InteractionInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Data/CharacterDataStruct.h"
#include "Weapons/WeaponBase.h"
#include "Characters/Components/StatusComponent.h"
#include "Characters/Components/WeaponSystemComponent.h"
#include "Components/InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Items/PickupItem/ResourcePickup.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialInstanceDynamic.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InitializeCharacterComponents();
	
}

void APlayerCharacter::DefaultWeaponEquip()
{
	if (WeaponComponent2)
	{
		// 1. 캐릭터 데이터 테이블에서 Row 정보 가져오기
		FCharacterData* RowData = CharacterDataHandle.GetRow<FCharacterData>(TEXT("InitWeapon"));

		// 2. 데이터가 유효하고, 설정된 기본 무기(BaseWeapon)가 있는지 확인
		if (RowData && !RowData->BaseWeapon.IsNull())
		{
			// 3. 무기 클래스 동기 로드 (SoftClassPtr -> UClass*)
			UClass* WeaponClass = RowData->BaseWeapon.LoadSynchronous();

			if (WeaponClass)
			{
				// 4. 해당 클래스와 일치하는 무기 ID(RowName) 검색
				FName WeaponID = WeaponComponent2->ResolveWeaponIDFromClass(WeaponClass);

				// 5. ID가 유효하면 무기 시스템에 최종 추가
				if (!WeaponID.IsNone())
				{
					WeaponComponent2->AddWeapon(WeaponID);
				}
			}
		}
	}
}

void APlayerCharacter::InitializeCharacterComponents()
{
	//컴포넌트 생성 Attach 및 위치 , 회전 조정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 100.0f);
	SpringArm->AddRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	

	PickupCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PickupCollision"));
	PickupCollision->SetupAttachment(RootComponent);
	PickupCollision->SetCapsuleSize(100.0f, 100.0f);


	StatusComponent2 = CreateDefaultSubobject<UStatusComponent>(TEXT("Status"));
	WeaponComponent2 = CreateDefaultSubobject<UWeaponSystemComponent>(TEXT("WeaponSystem"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -70.0f), FRotator(0.0f, -90.0f, 0.0f));

	//컴포넌트 세부 수치 설정
	GetCapsuleComponent()->SetCapsuleSize(30.0f, 70.0f);
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->SetWalkableFloorAngle(46.0f);

	DeathTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("DeathTimelineComp"));
	
	
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//커스텀 뎁스 활성화 (포스트 프로세스에서 이 메시를 구별할 수 있게 함)
	GetMesh()->SetRenderCustomDepth(true);

	GetMesh()->SetCustomDepthStencilValue(1);

	if (DeathCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleDeathProgress")); // 함수 연결
		DeathTimelineComp->AddInterpFloat(DeathCurve, ProgressFunction);    // 커브 연결
		DeathTimelineComp->SetLooping(false);
		DeathTimelineComp->SetIgnoreTimeDilation(true); // 시간 정지(Stopwatch) 스킬 써도 죽는 건 정상 속도로

		FOnTimelineEvent TimelineFinished;
		TimelineFinished.BindUFunction(this, FName("OnDeathTimelineFinished"));
		DeathTimelineComp->SetTimelineFinishedFunc(TimelineFinished);
	}
	
	OnActorBeginOverlap.AddDynamic(this, &APlayerCharacter::OnPickupOverlap);



	if (StatusComponent2)
	{
		// CharacterDataHandle 안에 테이블과 RowName이 다 들어있으므로 이것만 넘기면 끝!
		StatusComponent2->InitializeStatsFromDataTable(CharacterDataHandle);
		StatusComponent2->OnPlayerDied.AddDynamic(this, &APlayerCharacter::OnCharacterDie);
	}

	if (WeaponComponent2) {
		FCharacterData* rowData = CharacterDataHandle.GetRow<FCharacterData>(TEXT("Get Weapon"));
		if (rowData)
		{
			// 2. SoftClassPtr 로드 (비동기 로드면 로직이 다르지만, 여기선 동기 로드 사용)
			UClass* WeaponClass = rowData->BaseWeapon.LoadSynchronous();

			if (WeaponClass)
			{
				// 3. [핵심] 클래스 정보 -> 무기 ID(FName)로 변환
				FName WeaponID = WeaponComponent2->ResolveWeaponIDFromClass(WeaponClass);

				// 4. ID가 유효하면 무기 추가
				if (!WeaponID.IsNone())
				{
					// UE_LOG(LogTemp, Warning, TEXT("AddWeapon ID : %s"), *WeaponID.ToString());
					WeaponComponent2->AddWeapon(WeaponID);
				}
			}
		}
		
	}
	//피격시 효과 설정 위한 마테리얼 생성
	if (GetMesh())
	{
		int32 MaterialCount = GetMesh()->GetNumMaterials();
		for (int32 i = 0; i < MaterialCount; i++)
		{
			UMaterialInstanceDynamic* DMI = GetMesh()->CreateDynamicMaterialInstance(i);
			if (DMI)
			{
				MeshDMIs.Add(DMI);
			}
		}
	}

	DefaultWeaponEquip();

}

void APlayerCharacter::OnMoveInput(const FInputActionValue& InValue)
{
	FVector2D input = InValue.Get<FVector2D>();
	//입력 축 방향 반대로 해야 정방향
	FVector direction = FVector(input.Y, input.X,0.0f);

	//컨트롤러의 Yaw값을 가져와서 회전방향만큼 회전하여 바라보고 있는 방향으로 움직이게함
	FRotator ControllerRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	direction = ControllerRotation.RotateVector(direction);

	AddMovementInput(direction);
	
}

void APlayerCharacter::OnJumpInput(const FInputActionValue& InValue)
{
	Jump();
}

void APlayerCharacter::HandleDeathProgress(float Value)
{
	FRotator NewRot = FMath::Lerp(DeathStartRot, DeathEndRot, Value);
	SetActorRotation(NewRot);

	if (GlobalEffectMPC)
	{
		//UE_LOG(LogTemp, Warning, TEXT("SetScalarParameterValue : %f"), UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), GlobalEffectMPC,FName("FadeAlpha")));

		float Radius = FMath::Lerp(0.0f, 1.0f, Value);
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), GlobalEffectMPC, FName("FadeAlpha"), Radius);
	}

}

void APlayerCharacter::OnDeathTimelineFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("죽음 UI 실행"));
}

void APlayerCharacter::ResetHitFlash()
{
	for (auto* DMI : MeshDMIs)
	{
		if (DMI)
		{
			// 밝기를 0으로 돌려놓음
			DMI->SetScalarParameterValue(FName("HitAlpha"), 0.0f);
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//입력 액션 바인드 함수
	if (enhanced) {
		if (IA_Move) {
			enhanced->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::OnMoveInput);
		}
		if (IA_Jump) {
			enhanced->BindAction(IA_Jump, ETriggerEvent::Started, this, &APlayerCharacter::OnJumpInput);
		}
		if (IA_Interact) {
			enhanced->BindAction(IA_Interact, ETriggerEvent::Started, this, &APlayerCharacter::TryInteract);
		}
	}

}

void APlayerCharacter::TryInteract()
{
	//검사할 오브젝트 타입류
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	//무시할 액터종류
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(this);

	TArray<FHitResult> outResults;

	UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		GetActorLocation(), 
		GetActorLocation(),
		100.0f, 
		objectTypes, 
		false, 
		ignoreActors, 
		EDrawDebugTrace::ForDuration, 
		outResults, 
		true, 
		FLinearColor::Red,
		FLinearColor::Green, 
		5.0f);

	
	for (const auto& elements : outResults) {
		if (IsValid(elements.GetActor())) {
			//인터페이스 구현부 체크
			if (elements.GetActor()->Implements<UInteractionInterface>()) {
				IInteractionInterface::Execute_Interact(elements.GetActor(), this);
				//UE_LOG(LogTemp, Warning, TEXT("상호작용 : %s"), *elements.GetActor()->GetName());
			}
		}
	}
}

void APlayerCharacter::OnCharacterDie()
{
	UE_LOG(LogTemp, Warning, TEXT("플레이어 죽음: 사망 연출 시작"));

	//입력 차단
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}

	//충돌 끄기
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately(); // 즉시 정지
		GetCharacterMovement()->DisableMovement();         // 이동 기능 비활성화
		GetCharacterMovement()->bOrientRotationToMovement = false; // 이동 방향 회전 끄기
	}

	if (GetMesh()) {
		GetMesh()->SetAnimInstanceClass(nullptr);
	}

	//회전 목표 설정 (옆으로 90도)
	DeathStartRot = GetActorRotation();
	DeathEndRot = DeathStartRot;

	// 옆으로 눕히려면 Roll을 수정 (캐릭터 방향에 따라 Pitch일 수도 있음)
	DeathEndRot.Roll += 90.0f;
	// 만약 앞/뒤로 눕히고 싶다면: DeathEndRot.Pitch += 90.0f;

	//타임라인 재생!
	if (DeathTimelineComp && DeathCurve)
	{
		UE_LOG(LogTemp, Warning, TEXT("타임라인 재생"));
		DeathTimelineComp->PlayFromStart();
	}

}

void APlayerCharacter::ReceiveItem_Implementation(FName ItemID, int32 Count)
{
	if (InventoryComponent)
	{
		// 단순 AddItem 호출
		InventoryComponent->AddItem(ItemID, Count);
	}
}

float APlayerCharacter::GetAdjustedCost_Implementation(float BaseCost)
{
	if (StatusComponent2)
	{
		return StatusComponent2->CalculateChestCost(BaseCost);
	}
	return BaseCost;
}

bool APlayerCharacter::UseGold_Implementation(float Amount)
{
	if (StatusComponent2) return StatusComponent2->SpendGold(Amount);
	return 0;
}

void APlayerCharacter::NotifyChestOpened_Implementation()
{
	if (StatusComponent2)
	{
		StatusComponent2->IncreaseChestOpenCount();
	}
}

void APlayerCharacter::ActivateMagnetEffect()
{
	UE_LOG(LogTemp, Log, TEXT("자석 효과"));

	TArray<AActor*> FoundItems;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AResourcePickup::StaticClass(), FoundItems);

	for (AActor* Item : FoundItems)
	{
		if (AResourcePickup* resourceItem = Cast<AResourcePickup>(Item))
		{
			if (resourceItem->ResourceType == EResourceType::Exp && !resourceItem->IsPickup())
			{
				IPickupInterface::Execute_OnPickup(resourceItem, this);
			}
		}
	}
}

void APlayerCharacter::ActivateInvincibility(float Duration)
{
	if (bIsInvincible)
	{
		GetWorldTimerManager().ClearTimer(InvincibleTimerHandle);
	}
	bIsInvincible = true;

	UE_LOG(LogTemp, Log, TEXT("무적 상태 시작"));

	GetWorldTimerManager().SetTimer(InvincibleTimerHandle, this, &APlayerCharacter::OnInvincibleEnd, Duration, false);
}

void APlayerCharacter::OnInvincibleEnd()
{
	bIsInvincible = false;
	// TODO: 무적 이펙트 끄기
	UE_LOG(LogTemp, Log, TEXT("무적 상태 종료."));
}

void APlayerCharacter::ActivateStopwatch(float Duration)
{
	// 이미 시간 정지 중이면 타이머만 연장하고 리턴
	if (GetWorldTimerManager().IsTimerActive(StopwatchTimerHandle))
	{
		GetWorldTimerManager().SetTimer(StopwatchTimerHandle, this, &APlayerCharacter::OnStopwatchEnd, Duration, false);
		return;
	}

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.05f);

	this->CustomTimeDilation = 20.0f;

	UE_LOG(LogTemp, Log, TEXT("시간 정지 시작"));

	GetWorldTimerManager().SetTimer(StopwatchTimerHandle, this, &APlayerCharacter::OnStopwatchEnd, Duration * 0.05f, false);
}

void APlayerCharacter::OnStopwatchEnd()
{
	// 원래대로 복구
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	this->CustomTimeDilation = 1.0f;

	UE_LOG(LogTemp, Log, TEXT("시간 정지 종료."));
}

void APlayerCharacter::OnPickupOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//인터페이스 구현부 체크
	if (OtherActor->Implements<UPickupInterface>()) {
		UE_LOG(LogTemp, Warning, TEXT("Pickup : %s"), *OtherActor->GetName());
		IPickupInterface::Execute_OnPickup(OtherActor,this);
	}
	
	
}


float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float resultarmor = 1.0f - StatusComponent2->GetResultArmor();
	
	float finalTakeDamage = DamageAmount* resultarmor;

	if (bIsInvincible)
	{
		finalTakeDamage = 0.0f;
	}
	if (InventoryComponent)
	{
		UE_LOG(LogTemp, Warning,TEXT("피격시 트리거 함수 실행"));
		InventoryComponent->ProcessProcTrigger(EProcTriggerType::OnTakeDamage, DamageCauser, finalTakeDamage);
	}


	if (finalTakeDamage > 0.0f) {
		StatusComponent2->AddCurrentHP(-finalTakeDamage);

		for (auto* DMI : MeshDMIs)
		{
			if (DMI)
			{
				//UE_LOG(LogTemp, Warning, TEXT("DMI"));
				// 색상을 빨간색으로 설정 (Red)
				DMI->SetVectorParameterValue(FName("HitColor"), FLinearColor::Red);
				// 밝기를 1로 설정
				DMI->SetScalarParameterValue(FName("HitAlpha"), 1.0f);
			}
		}

		//기존 타이머 초기화 (연타 맞았을 때 깜빡임 끊기지 않게)
		GetWorldTimerManager().ClearTimer(HitFlashTimerHandle);

		//ResetHitFlash 함수 (다시 돌리기)
		GetWorldTimerManager().SetTimer(HitFlashTimerHandle, this, &APlayerCharacter::ResetHitFlash, 0.2f, false);

		UE_LOG(LogTemp, Warning, TEXT("%.1f / %.1f"), StatusComponent2->GetCurrentHP(), StatusComponent2->GetResultMaxHP());
	}
	

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return finalTakeDamage;
}


