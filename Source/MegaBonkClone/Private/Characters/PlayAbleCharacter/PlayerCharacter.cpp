// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Components/StatusComponent.h"
#include "Characters/Components/WeaponSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"

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
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("Status"));
	WeaponComponent2 = CreateDefaultSubobject<UWeaponSystemComponent>(TEXT("WeaponSystem"));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -70.0f), FRotator(0.0f, -90.0f, 0.0f));

	//컴포넌트 세부 수치 설정
	GetCapsuleComponent()->SetCapsuleSize(30.0f, 70.0f);
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->SetWalkableFloorAngle(46.0f);
	//점프횟수 조절가능
	JumpMaxCount = 2;
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &APlayerCharacter::OnBeginOverlap);

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
		enhanced->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::OnMoveInput);

		enhanced->BindAction(IA_Jump, ETriggerEvent::Started, this, &APlayerCharacter::OnJumpInput);

	}

}

void APlayerCharacter::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("오버랩 성공 : %s"), *OtherActor->GetName());
}

void APlayerCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("히트 성공 : %s"), *Other->GetName());
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return 0.0f;
}

