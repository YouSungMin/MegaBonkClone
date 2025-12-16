// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/InventoryOwner.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UStatusComponent;
UCLASS()
class MEGABONKCLONE_API APlayerCharacter : public ACharacter, public IInventoryOwner
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	//캐릭터 초기 설정 함수
	void InitializeCharacterComponents();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void TryInteract();

	virtual void ReceiveItem_Implementation(FName ItemID, int32 Count) override;

	//스테이터스 GETTER 함수
	inline UStatusComponent* GetStatusComponent() { return StatusComponent; }

	//오브젝트에 오버랩 되었을때 실행할 함수
	UFUNCTION()
	void OnPickupOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	virtual void NotifyHit(
		class UPrimitiveComponent* MyComp,
		AActor* Other,
		class UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit
	) override;
	//데미지 처리 함수
	UFUNCTION()
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//이동 입력 받으면 실행할 함수
	UFUNCTION()
	void OnMoveInput(const FInputActionValue& InValue);
	//점프 입력 받으면 실행할 함수
	UFUNCTION()
	void OnJumpInput(const FInputActionValue& InValue);
	
	/*UFUNCTION()
	void */
private:

public:

protected:

	//입력 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
	TObjectPtr<class UInputAction> IA_Move = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
	TObjectPtr<class UInputAction> IA_Jump = nullptr;


	//컴포넌트들 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Components")
	TObjectPtr<class USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Components")
	TObjectPtr<class UCameraComponent> Camera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Components")
	TObjectPtr<class UCapsuleComponent> PickupCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|Components")
	TObjectPtr<UStatusComponent> StatusComponent = nullptr;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Player|Components")
	TObjectPtr<class UWeaponSystemComponent> WeaponComponent = nullptr;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Player|Components")
	TObjectPtr<class UInventoryComponent> InventoryComponent = nullptr;


private:


};
