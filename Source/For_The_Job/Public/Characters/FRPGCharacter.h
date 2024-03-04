#pragma once

#include "CoreMinimal.h"
#include "Characters/FCharacter.h"
#include "InputActionValue.h"
#include "FRPGCharacter.generated.h"

class UFInputConfigData;
class UInputComponent;
class UInputMappingContext;
class AFItem;

UENUM()
enum class EViewMode : uint8 {
	None,
	BackView,
	QuarterView,
	End
};

UCLASS()
class FOR_THE_JOB_API AFRPGCharacter : public AFCharacter {
	GENERATED_BODY()
	
public:
	AFRPGCharacter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	void SetViewMode(EViewMode InViewMode);

	FORCEINLINE void SetOverlappingItem(AFItem *Item) { OverlappingItem = Item; }

protected:
	virtual void BeginPlay() override;

private:
	void Move(const FInputActionValue& InValue);

	void Look(const FInputActionValue& InValue);

	void ChangeView(const FInputActionValue& InValue);

	void Run(const FInputActionValue& InValue);

	virtual void Jump() override;

	void StartJump();

	void StopJump();

	void EKeyPressed();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RPGCharacter", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UFInputConfigData> PlayerCharacterInputConfigData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RPGCharacter", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> PlayerCharacterInputMappingContext;

	EViewMode CurrentViewMode = EViewMode::None;

	FVector DirectionToMove = FVector::ZeroVector;

	float DestArmLength = 0.f;

	float ArmLengthChangeSpeed = 3.f;

	FRotator DestArmRotation = FRotator::ZeroRotator;

	float ArmRotationChangeSpeed = 10.f;

	bool IsWalking = true;

	float RunSpeed = 500.f;

	float WalkSpeed = 300.f;

	float DefaultGravityScale = 1.f;

	float GravityScaleOnJumpStart = 1.5f;

	UPROPERTY(VisibleInstanceOnly)
	AFItem *OverlappingItem;
};
