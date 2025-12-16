// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickupItem/PickupBaseActor.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
APickupBaseActor::APickupBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseRoot = CreateDefaultSubobject<USphereComponent>(TEXT("BaseRoot"));
	SetRootComponent(BaseRoot);
	BaseRoot->InitSphereRadius(1.0f);
	BaseRoot->SetSimulatePhysics(true);
	BaseRoot->BodyInstance.bLockXRotation = true;
	BaseRoot->BodyInstance.bLockYRotation = true;
	BaseRoot->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	BaseRoot->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	BaseRoot->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

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
		PickupTimeline->SetPlayRate(1 / Duration);
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

	//if (!bIsBillboard)
	//{
	//	Mesh->AddWorldRotation(FRotator(0, RotateSpeed * DeltaTime, 0));
	//}
	if(bIsBillboard)
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
	if (!bPickuped)
	{
		// 먹기 처리
		UE_LOG(LogTemp, Log, TEXT("OnPickup_Implementation 실행"));
		bPickuped = true;
		PickupOwner = Target;
		PickupStartLocation = GetActorLocation();
		SetActorEnableCollision(false);		// 이 액터와 액터가 포함하는 모든 컴포넌트의 충돌 정지
		BaseRoot->SetSimulatePhysics(false);// 바닥으로 가라앉는것 방지
		PickupTimeline->PlayFromStart();	// 타임라인 시작
	}
}

void APickupBaseActor::OnPickupComplete_Implementation()
{
	Destroy();
}

void APickupBaseActor::OnTimelineUpdate(float Value)
{
	// 플레이어가 유효하지 않으면 아이템을 삭제
	if (!PickupOwner.IsValid())
	{
		Destroy();
		return;
	}
	// 타임라인의 정규화 된 진행 시간(0~1)
	float currentTime = PickupTimeline->GetPlaybackPosition();

	// 커브의 현재 값 받아오기
	float distanceValue = Value;
	//float heightValue = HeightCurve ? HeightCurve->GetFloatValue(currentTime) : 0.0f;
	float scaleValue = ScaleCurve ? ScaleCurve->GetFloatValue(currentTime) : 1.0f;

	// 커브값을 기준으로 새 위치와 스케일 계산
	FVector NewLocation = FMath::Lerp(PickupStartLocation, PickupOwner.Get()->GetActorLocation(), distanceValue);
	NewLocation += PickupHeight * FVector::UpVector;
	//NewLocation += heightValue * PickupHeight * FVector::UpVector;
	SetActorLocation(NewLocation);

	FVector NewScale = FVector::One() * scaleValue;
	Mesh->SetRelativeScale3D(NewScale);
}

void APickupBaseActor::OnTimelineFinished()
{
	Execute_OnPickupComplete(this);
}