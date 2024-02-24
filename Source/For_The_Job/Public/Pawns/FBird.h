#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FBird.generated.h"

// 전방 선언으로 헤더 파일 의존성과 빌드 시간 감소
class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class FOR_THE_JOB_API AFBird : public APawn {
	GENERATED_BODY()

public:
	AFBird();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void Turn(float Value);

	void LookUp(float Value);

private:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent *Capsule;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent *BirdMesh;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
};
 