#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/CharacterType.h"
#include "FAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckHitDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisarmDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArmDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishEquippingDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnableBoxCollisionDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisbleBoxCollisionDelegate);

UCLASS()
class FOR_THE_JOB_API UFAnimInstance : public UAnimInstance {
	GENERATED_BODY()

	friend class AFRPGCharacter;
	
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	void PlayAttackMontage();

	UFUNCTION()
	void AnimNotify_CheckHit();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	void PlayEquipMontage(const FName &SectionName);

	UFUNCTION()
	void AnimNotify_Disarm();

	UFUNCTION()
	void AnimNotify_Arm();

	UFUNCTION()
	void AnimNotify_FinishEquipping();

	UFUNCTION()
	void AnimNotify_EnableBoxCollision();

	UFUNCTION()
	void AnimNotify_DisableBoxCollision();


public:
	UPROPERTY(BlueprintReadOnly)
	class AFRPGCharacter *RPGCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent *RPGCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	uint8 IsFalling : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	ECharacterState CharacterState;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage *AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage *EquipMontage;

	FOnCheckHitDelegate OnCheckHitDelegate;

	FOnAttackEndDelegate OnAttackEndDelegate;

	FOnDisarmDelegate OnDisarmDelegate;

	FOnArmDelegate OnArmDelegate;

	FOnFinishEquippingDelegate OnFinishEquippingDelegate;

	FOnEnableBoxCollisionDelegate OnEnableBoxCollisionDelegate;

	FOnDisbleBoxCollisionDelegate OnDisbleBoxCollisionDelegate;
};
