#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterType.h"
#include "Interfaces/PickupInterface.h"
#include "RPGCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItem;
class ASoul;
class ATreasure;
class UAnimMontage;
class URPGOverlay;

UCLASS()
class FOR_THE_JOB_API ARPGCharacter : public ABaseCharacter, public IPickupInterface {
	GENERATED_BODY()

public:
	ARPGCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	virtual void Jump() override;

	void StartJump();
	
	void StopJump();

	void WalkRun();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;
	
	virtual void GetHit_Implementation(const FVector &ImpactPoint, AActor *Hitter) override;
	
	virtual void SetOverlappingItem(AItem *Item) override;
	
	virtual void AddSouls(ASoul *Soul) override;
	
	virtual void AddGold(ATreasure *Treasure) override;

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);
	
	void Turn(float Value);
	
	void LookUp(float Value);

	void EKeyPressed();

	virtual void Attack() override;
	
	void Dodge();

	void EquipWeapon(AWeapon *Weapon);
	
	virtual void AttackEnd() override;
	
	virtual void DodgeEnd() override;
	
	virtual bool CanAttack() override;
	
	bool CanDisarm();
	
	bool CanArm();
	
	void Disarm();
	
	void Arm();
	
	void PlayEquipMontage(const FName &SectionName);
	
	//virtual void Die_Implementation() override;
	
	bool HasEnoughStamina();
	
	bool IsOccupied();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

private:
	bool IsUnoccupied();
	
	void InitializeRPGOverlay();
	
	void SetHUDHealth();

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent *CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent *ViewCamera;

	bool bJumpInput = false;

	bool bIsRunning = false;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	float WalkSpeed = 300.f;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	float RunSpeed = 500;

	UPROPERTY(VisibleInstanceOnly)
	AItem *OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage *EquipMontage;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY()
	URPGOverlay *RPGOverlay;

public:
	FORCEINLINE bool GetJumpInput() const { return bJumpInput; }

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
