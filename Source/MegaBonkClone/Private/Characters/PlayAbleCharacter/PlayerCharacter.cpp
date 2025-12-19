// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
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
#include <Kismet/GameplayStatics.h>

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InitializeCharacterComponents();
	
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


	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("Status"));
	WeaponComponent = CreateDefaultSubobject<UWeaponSystemComponent>(TEXT("WeaponSystem"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -70.0f), FRotator(0.0f, -90.0f, 0.0f));

	//컴포넌트 세부 수치 설정
	GetCapsuleComponent()->SetCapsuleSize(30.0f, 70.0f);
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->SetWalkableFloorAngle(46.0f);
	//점프횟수 조절가능
	//JumpMaxCount = 2;
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &APlayerCharacter::OnPickupOverlap);



	if (StatusComponent)
	{
		// CharacterDataHandle 안에 테이블과 RowName이 다 들어있으므로 이것만 넘기면 끝!
		StatusComponent->InitializeStatsFromDataTable(CharacterDataHandle);
		StatusComponent->OnPlayerDied.AddDynamic(this, &APlayerCharacter::OnCharacterDie);
	}

	if (WeaponComponent) {
		FCharacterData* rowData = CharacterDataHandle.GetRow<FCharacterData>(TEXT("Get Weapon"));
		if (rowData) {
			
			//UE_LOG(LogTemp, Warning, TEXT("AddWeapon : %s"), *rowData->BaseWeapon->GetName());
			WeaponComponent->AddWeapon(rowData->BaseWeapon.LoadSynchronous());
		}
		
	}

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
	UE_LOG(LogTemp, Warning, TEXT("플레이어 죽음"));

	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}

	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	if (StatusComponent)
	{
		return StatusComponent->CalculateChestCost(BaseCost);
	}
	return BaseCost;
}

bool APlayerCharacter::UseGold_Implementation(float Amount)
{
	if (StatusComponent) return StatusComponent->SpendGold(Amount);
	return 0;
}

void APlayerCharacter::NotifyChestOpened_Implementation()
{
	if (StatusComponent)
	{
		StatusComponent->IncreaseChestOpenCount();
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

void APlayerCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Other && Other->ActorHasTag("Enemy")) {
		//충돌 데미지 처리
		//UE_LOG(LogTemp, Warning, TEXT("히트 성공 : %s"), *Other->GetName());
	}
	
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float resultarmor = 1.0f - StatusComponent->GetResultArmor();
	
	float finalTakeDamage = DamageAmount* resultarmor;

	if (bIsInvincible)
	{
		finalTakeDamage = 0.0f;
	}
	StatusComponent->AddCurrentHP(-finalTakeDamage);
	UE_LOG(LogTemp, Warning, TEXT("%.1f / %.1f"), StatusComponent->GetCurrentHP(), StatusComponent->GetResultMaxHP());

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return finalTakeDamage;
}


