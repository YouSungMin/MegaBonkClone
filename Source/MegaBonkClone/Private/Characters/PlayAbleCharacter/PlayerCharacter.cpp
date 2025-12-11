// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayAbleCharacter/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
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
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 100.0f);
	SpringArm->AddRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -70.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetCapsuleComponent()->SetCapsuleSize(30.0f, 70.0f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
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

		enhanced->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &APlayerCharacter::OnJumpInput);

	}

}

