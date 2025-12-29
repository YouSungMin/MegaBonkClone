#include "Items/PickupItem/PickupBaseActor.h"
#include "Components/SphereComponent.h"
#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Components/TimelineComponent.h"
#include "Framework/ObjectPoolSubsystem.h"
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

	PickupTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PickupTimeline"));
}

// Called when the game starts or when spawned
void APickupBaseActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[PICKUP_DEBUG] BeginPlay 시작: %s (위치: %s)"), *GetName(), *GetActorLocation().ToString());

	FTimerHandle WaitHandle;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, [this]()
		{
			if (!IsValid(BaseRoot))
			{
				UE_LOG(LogTemp, Error, TEXT("[PICKUP_DEBUG] BeginPlay 타이머: BaseRoot가 유효하지 않음!"));
				return;
			}

			TArray<AActor*> OverlappingActors;
			BaseRoot->GetOverlappingActors(OverlappingActors);

			// 디버그 로그: 감지된 액터 개수 확인
			UE_LOG(LogTemp, Warning, TEXT("[PICKUP_DEBUG] BeginPlay 타이머 실행됨. 감지된 오버랩 액터 수: %d"), OverlappingActors.Num());

			for (AActor* Actor : OverlappingActors)
			{
				UE_LOG(LogTemp, Log, TEXT("[PICKUP_DEBUG]  - 감지된 액터: %s (클래스: %s)"), *Actor->GetName(), *Actor->GetClass()->GetName());

				// 플레이어인지 확인
				if (Actor && Actor->IsA(ACharacter::StaticClass()))
				{
					UE_LOG(LogTemp, Warning, TEXT("[PICKUP_DEBUG]  >> 플레이어(Character) 감지됨! 강제 줍기 시도: %s"), *Actor->GetName());

					if (this->GetClass()->ImplementsInterface(UPickupInterface::StaticClass()))
					{
						IPickupInterface::Execute_OnPickup(this, Actor);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("[PICKUP_DEBUG]  >> 오류: 이 액터는 UPickupInterface를 상속받지 않았습니다!"));
					}
					// 찾았으면 루프 종료
					break;
				}
			}
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
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[PICKUP_DEBUG] Timeline Curve가 설정되지 않았습니다! (ScaleCurve or DistanceCurve is NULL)"));
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
	UE_LOG(LogTemp, Warning, TEXT("[PICKUP_DEBUG] OnPickup_Implementation 호출됨. 요청자: %s"), Target ? *Target->GetName() : TEXT("NULL"));

	if (!Target || !Target->IsA(APlayerCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("[PICKUP_DEBUG] OnPickup 실패: Target이 없거나 PlayerCharacter가 아님. (Target Class: %s)"), Target ? *Target->GetClass()->GetName() : TEXT("NULL"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[PICKUP_DEBUG] OnPickup 상태 체크 -> bPickuped: %s"), bPickuped ? TEXT("TRUE (이미 먹힘)") : TEXT("FALSE (먹기 가능)"));

	if (!bPickuped)
	{
		// 먹기 처리
		UE_LOG(LogTemp, Warning, TEXT("[PICKUP_DEBUG] 아이템 획득 로직 시작 (bPickuped = true 설정)"));
		bPickuped = true;
		PickupOwner = Target;
		PickupStartLocation = GetActorLocation();

		if (BaseRoot) BaseRoot->SetSimulatePhysics(false); // 바닥으로 가라앉는것 방지

		if (PickupTimeline)
		{
			UE_LOG(LogTemp, Log, TEXT("[PICKUP_DEBUG] 타임라인 PlayFromStart 실행"));
			PickupTimeline->PlayFromStart();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[PICKUP_DEBUG] 타임라인 컴포넌트가 없습니다!"));
		}
	}
}

void APickupBaseActor::OnPickupComplete_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[PICKUP_DEBUG] OnPickupComplete 실행 (획득 완료 처리)"));

	UWorld* World = GetWorld();
	if (World)
	{
		UObjectPoolSubsystem* Pool = World->GetSubsystem<UObjectPoolSubsystem>();
		if (Pool)
		{
			UE_LOG(LogTemp, Log, TEXT("[PICKUP_DEBUG] ReturnToPool 호출"));
			Pool->ReturnToPool(this);
			return;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("[PICKUP_DEBUG] Pool Subsystem을 찾을 수 없어 Destroy() 호출"));
	Destroy();
}

void APickupBaseActor::OnPoolActivate_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[PICKUP_DEBUG] OnPoolActivate (풀에서 꺼내짐) : %s"), *GetName());

	// 1. [중요] 가장 먼저 액터를 보이게 설정
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	// 2. 로직 플래그 초기화
	bPickuped = false;
	PickupOwner = nullptr;
	PickupStartLocation = FVector::ZeroVector;

	// 3. 컴포넌트(루트) 물리 및 콜리전 재설정
	if (BaseRoot)
	{
		// (1) 물리 끄기
		BaseRoot->SetSimulatePhysics(false);

		// (2) 콜리전 프리셋 설정
		BaseRoot->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BaseRoot->SetCollisionObjectType(ECC_WorldDynamic);

		// (3) 채널 반응 설정
		BaseRoot->SetCollisionResponseToAllChannels(ECR_Ignore);

		BaseRoot->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		BaseRoot->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		BaseRoot->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		BaseRoot->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		BaseRoot->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		BaseRoot->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);

		// (4) 물리 켜기
		BaseRoot->SetSimulatePhysics(true);

		// (5) 속도 초기화
		BaseRoot->SetPhysicsLinearVelocity(FVector::ZeroVector);
		BaseRoot->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

		BaseRoot->WakeRigidBody();

		// [중요] OnPoolActivate 내의 타이머 로직에도 로그 추가
		FTimerHandle WaitHandle;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, [this]() {
			if (!IsValid(BaseRoot)) return;
			TArray<AActor*> OverlappingActors;
			BaseRoot->GetOverlappingActors(OverlappingActors);

			UE_LOG(LogTemp, Log, TEXT("[PICKUP_DEBUG] PoolActivate 타이머 체크: 감지된 수 %d"), OverlappingActors.Num());

			for (AActor* Actor : OverlappingActors)
			{
				if (Actor && Actor->IsA(ACharacter::StaticClass()))
				{
					UE_LOG(LogTemp, Warning, TEXT("[PICKUP_DEBUG] PoolActivate 즉시 줍기: %s"), *Actor->GetName());
					if (this->GetClass()->ImplementsInterface(UPickupInterface::StaticClass()))
					{
						IPickupInterface::Execute_OnPickup(this, Actor);
					}
					break;
				}
			}
			}, 0.5f, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[PICKUP_DEBUG] BaseRoot가 없습니다! (OnPoolActivate)"));
	}

	if (Mesh)
	{
		Mesh->SetRelativeScale3D(FVector(0.3f));
	}

	if (PickupTimeline)
	{
		PickupTimeline->Stop();
		PickupTimeline->SetPlaybackPosition(0.0f, false);
	}
}

void APickupBaseActor::OnPoolDeactivate_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[PICKUP_DEBUG] OnPoolDeactivate (풀로 돌아감) : %s"), *GetName());

	if (BaseRoot)
	{
		BaseRoot->SetSimulatePhysics(false);
		BaseRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BaseRoot->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	if (PickupTimeline)
	{
		PickupTimeline->Stop();
	}

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void APickupBaseActor::OnTimelineUpdate(float Value)
{
	if (!bPickuped)
	{
		// 이 로그가 계속 뜨면 타임라인이 도는데 bPickuped가 false인 이상한 상황
		// UE_LOG(LogTemp, Warning, TEXT("[PICKUP_DEBUG] Timeline 돌고 있는데 bPickuped가 false임. 강제 중지.")); 
		if (PickupTimeline && PickupTimeline->IsPlaying())
		{
			PickupTimeline->Stop();
		}
		return;
	}

	if (!PickupOwner.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[PICKUP_DEBUG] 주인이 사라짐(Destroy됨). 아이템 삭제."));
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
	UE_LOG(LogTemp, Warning, TEXT("[PICKUP_DEBUG] Timeline 종료. OnPickupComplete 호출."));
	Execute_OnPickupComplete(this);
}