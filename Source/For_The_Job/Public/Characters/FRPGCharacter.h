#pragma once

#include "CoreMinimal.h"
#include "Characters/FCharacter.h"
#include "InputActionValue.h"
#include "Characters/CharacterType.h"
#include "FRPGCharacter.generated.h"

class UFInputConfigData;
class UInputComponent;
class UInputMappingContext;
class UAnimMontage;
class AFItem;
class AFWeapon;

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

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage *Montage, bool bInterrupted);

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

	void Attack();

	bool CanAttack();

	UFUNCTION()
	void CheckHit();

	void BeginCombo();

	UFUNCTION()
	void CheckCanNextCombo();

	UFUNCTION()
	void EndCombo(UAnimMontage *InAnimMontage, bool bInterrupted);

	bool CanDisarm();

	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void EnableBoxCollision();

	UFUNCTION(BlueprintCallable)
	void DisableBoxCollision();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RPGCharacter, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UFInputConfigData> PlayerCharacterInputConfigData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RPGCharacter, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> PlayerCharacterInputMappingContext;

	// 부드러운 시점 전환을 위한 변수들 - - - - - - - - - - - - - -
	EViewMode CurrentViewMode = EViewMode::None;

	FVector DirectionToMove = FVector::ZeroVector;

	float DestArmLength = 0.f;

	float ArmLengthChangeSpeed = 3.f;

	FRotator DestArmRotation = FRotator::ZeroRotator;

	float ArmRotationChangeSpeed = 10.f;

	// 걷기/달리기 전환을 위한 변수들 - - - - - - - - - - - - - - - -
	bool IsWalking = true;

	float RunSpeed = 500.f;

	float WalkSpeed = 300.f;

	// 점프 시 빠른 착지를 위한 변수들 - - - - - - - - - - - - - - -  
	float DefaultGravityScale = 1.f;

	float GravityScaleOnJumpStart = 1.5f;

	// 아이템 상호작용을 위한 변수들 - - - - - - -- - - - - - - - - -
	UPROPERTY(VisibleInstanceOnly)
	AFItem *OverlappingItem;

	// 무기 소지에 따른 애니메이션 재생을 위한 변수들 - - - - - - - -
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AFWeapon *EquippedWeapon;

	// 콤보 공격을 위한 변수들 - - - - - - - - - - - - - - - - - - - 
	EActionState ActionState = EActionState::EAS_Unoccupied;

	FString AttackAnimMontageSectionName = FString(TEXT("Attack"));

	int32 MaxComboCount = 3;

	int32 CurrentComboCount = 0;

	bool bIsAttackKeyPressed = false;
};
