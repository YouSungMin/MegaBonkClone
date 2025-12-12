// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeaponBaseActor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"//임시

// Sets default values
AProjectileWeaponBaseActor::AProjectileWeaponBaseActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);
	BaseMesh->SetCollisionProfileName("NoCollision");

	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	Collision->SetupAttachment(BaseMesh);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileComponent->bIsHomingProjectile = true;
	RotatingComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingComponent"));

	RotatingComponent->RotationRate = FRotator(0.0f, 720.0f, 0.0f);



}
void AProjectileWeaponBaseActor::SetHomingTarget(AActor* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("SetHomingTarget"));
	ProjectileComponent->HomingTargetComponent=Target->GetRootComponent();
}

void AProjectileWeaponBaseActor::OnBeginWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnBeginWeaponOverlap(OverlappedActor, OtherActor);
}

void AProjectileWeaponBaseActor::OnEndWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnEndWeaponOverlap(OverlappedActor, OtherActor);
}

// Called when the game starts or when spawned
void AProjectileWeaponBaseActor::BeginPlay()
{
	Super::BeginPlay();
	SetHomingTarget(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}
