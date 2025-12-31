#include "Items/PickupItem/PickupBaseActor.h"
#include "Components/SphereComponent.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Components/TimelineComponent.h"
#include "Framework/ObjectPoolSubsystem.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickupBaseActor::APickupBaseActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseRoot = CreateDefaultSubobject<USphereComponent>(TEXT("BaseRoot"));
	SetRootComponent(BaseRoot);
	BaseRoot->InitSphereRadius(10.0f);
	BaseRoot->SetSimulatePhysics(true);
	BaseRoot->BodyInstance.bLockXRotation = true;
	BaseRoot->BodyInstance.bLockYRotation = true;
	BaseRoot->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	BaseRoot->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	BaseRoot->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BaseRoot);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->AddRelativeRotation(FRotator(0, 0, -10.0f));


	InteractionWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidgetComp->SetupAttachment(RootComponent); // 루트에 붙이기

	// ★핵심 설정: Screen 모드로 하면 카메라를 항상 정면으로 바라보고 크기가 일정하게 유지됨 (빌보드)
	InteractionWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	// 위젯 크기 설정 (원하는 대로 조절)
	InteractionWidgetComp->SetDrawAtDesiredSize(true);

	// 위치를 아이템 머리 위로 살짝 올림 (Z축 +80)
	InteractionWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));

	// 처음엔 안 보이게 숨김
	InteractionWidgetComp->SetVisibility(false);
	PickupTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PickupTimeline"));
}

// Called when the game starts or when spawned
void APickupBaseActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(CheckTimerHandle, [this]()
		{
			CheckOverlappingAndPickup();

		}, 0.5f, false);


	if (PickupTimeline)
	{
		if (ScaleCurve && DistanceCurve)
		{
			FOnTimelineFloat UpdateDelegate;
			UpdateDelegate.BindUFunction(this, FName("OnTimelineUpdate"));
			PickupTimeline->AddInterpFloat(DistanceCurve, UpdateDelegate);

			FOnTimelineEvent FinishedDelegate;
			FinishedDelegate.BindUFunction(this, FName("OnTimelineFinished"));
			PickupTimeline->SetTimelineFinishedFunc(FinishedDelegate);
		}

		float safeDuration = (Duration > 0.0f) ? Duration : 1.0f;
		PickupTimeline->SetPlayRate(1 / safeDuration);
	}

	if (Mesh)
	{
		InitialMeshRotation = Mesh->GetRelativeRotation();
	}
	bPickuped = false;
}

void APickupBaseActor::CheckOverlappingAndPickup()
{
	if (!IsValid(BaseRoot)) return;

	TArray<AActor*> OverlappingActors;
	BaseRoot->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->IsA(ACharacter::StaticClass()))
		{
			if (this->GetClass()->ImplementsInterface(UPickupInterface::StaticClass()))
			{
				IPickupInterface::Execute_OnPickup(this, Actor);
			}
			break;
		}
	}
}

// Called every frame
void APickupBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsBillboard)
	{
		if (APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
		{
			FRotator CamRot = CamManager->GetCameraRotation();
			FRotator NewRot = FRotator(InitialMeshRotation.Pitch, CamRot.Yaw + InitialMeshRotation.Yaw, InitialMeshRotation.Roll);
			Mesh->SetWorldRotation(NewRot);
		}
	}
}

void APickupBaseActor::OnPickup_Implementation(AActor* Target)
{
	// 아직 줍기 불가 상태면 무시
	if (!bCanBePickedUp)
	{
		return;
	}

	// 이미 먹혔으면 무시
	if (bPickuped) return;

	// 타겟 유효성 검사
	if (!Target || !Target->IsA(APlayerCharacter::StaticClass()))
	{
		return;
	}

	// 먹기 처리
	bPickuped = true;
	PickupOwner = Target;
	PickupStartLocation = GetActorLocation();

	if (BaseRoot) BaseRoot->SetSimulatePhysics(false);

	if (PickupTimeline)
	{
		PickupTimeline->PlayFromStart();
	}
}

void APickupBaseActor::OnPickupComplete_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UObjectPoolSubsystem* Pool = World->GetSubsystem<UObjectPoolSubsystem>();
		if (Pool)
		{
			Pool->ReturnToPool(this);
			return;
		}
	}

	Destroy();
}

void APickupBaseActor::OnPoolActivate_Implementation()
{
	// 변수 초기화
	bPickuped = false;
	bCanBePickedUp = false;
	PickupOwner = nullptr;
	PickupStartLocation = FVector::ZeroVector;

	// 액터 켜기
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	if (BaseRoot)
	{
		// 물리 상태 리셋
		BaseRoot->SetSimulatePhysics(false);
		BaseRoot->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BaseRoot->SetSimulatePhysics(true);

		// 속도 0으로 (안 튀게)
		BaseRoot->SetPhysicsLinearVelocity(FVector::ZeroVector);
		BaseRoot->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		BaseRoot->WakeRigidBody();
	}

	if (Mesh) Mesh->SetRelativeScale3D(FVector(0.3f));
	if (PickupTimeline)
	{
		PickupTimeline->Stop();
		PickupTimeline->SetPlaybackPosition(0.0f, false);
	}

	// 1초 뒤에 EnablePickup 함수 실행
	GetWorld()->GetTimerManager().SetTimer(EnablePickupTimerHandle, this, &APickupBaseActor::EnablePickup, 1.0f, false);
}

void APickupBaseActor::OnPoolDeactivate_Implementation()
{
	// 타이머 취소
	GetWorld()->GetTimerManager().ClearTimer(EnablePickupTimerHandle);

	bCanBePickedUp = false;

	if (BaseRoot)
	{
		BaseRoot->SetSimulatePhysics(false);
		BaseRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (PickupTimeline) PickupTimeline->Stop();

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void APickupBaseActor::BeginFocus_Implementation()
{
	if (InteractionWidgetComp)
	{
		InteractionWidgetComp->SetVisibility(true);
	}
}

void APickupBaseActor::EndFocus_Implementation()
{
	if (InteractionWidgetComp)
	{
		InteractionWidgetComp->SetVisibility(false);
	}
}

void APickupBaseActor::EnablePickup()
{
	bCanBePickedUp = true;

	if (IsValid(BaseRoot))
	{
		TArray<AActor*> OverlappingActors;
		BaseRoot->GetOverlappingActors(OverlappingActors);

		for (AActor* Actor : OverlappingActors)
		{
			if (Actor && Actor->IsA(APlayerCharacter::StaticClass()))
			{
				Execute_OnPickup(this, Actor);
				break;
			}
		}
	}
}

void APickupBaseActor::OnTimelineUpdate(float Value)
{
	if (!bPickuped)
	{
		if (PickupTimeline && PickupTimeline->IsPlaying())
		{
			PickupTimeline->Stop();
		}
		return;
	}

	if (!PickupOwner.IsValid())
	{
		Destroy();
		return;
	}

	float currentTime = PickupTimeline->GetPlaybackPosition();
	float distanceValue = Value;
	float scaleValue = ScaleCurve ? ScaleCurve->GetFloatValue(currentTime) : 1.0f;

	FVector NewLocation = FMath::Lerp(PickupStartLocation, PickupOwner.Get()->GetActorLocation(), distanceValue);
	NewLocation += PickupHeight * FVector::UpVector;

	SetActorLocation(NewLocation);

	FVector NewScale = FVector(scaleValue);
	Mesh->SetRelativeScale3D(NewScale);
}

void APickupBaseActor::OnTimelineFinished()
{
	Execute_OnPickupComplete(this);
}