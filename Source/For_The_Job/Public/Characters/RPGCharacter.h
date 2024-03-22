#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterType.h"
#include "RPGCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class FOR_THE_JOB_API ARPGCharacter : public ABaseCharacter {
	GENERATED_BODY()

public:
	ARPGCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	virtual void Jump() override;

	void StartJump();
	
	void StopJump();

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);
	
	void Turn(float Value);
	
	void LookUp(float Value);

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent *CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent *ViewCamera;

	bool bJumpInput;

public:
	FORCEINLINE bool GetJumpInput() const { return bJumpInput; }
};
