#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FCharacter.generated.h"

UCLASS()
class FOR_THE_JOB_API AFCharacter : public ACharacter {
	GENERATED_BODY()

public:
	AFCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void MoveForward(float Value);

protected:
	virtual void BeginPlay() override;
};
