#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FEnemy.generated.h"

UCLASS()
class FOR_THE_JOB_API AFEnemy : public ACharacter {
	GENERATED_BODY()

public:
	AFEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
};
