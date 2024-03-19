#pragma once

#include "CoreMinimal.h"
#include "Characters/FCharacter.h"
#include "Characters/CharacterType.h"
#include "FEnemy.generated.h"

class UFHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class FOR_THE_JOB_API AFEnemy : public AFCharacter {
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 5.f;

private:
	UPROPERTY(VisibleAnywhere)
	UFHealthBarComponent *HealthBarWidget;

	UPROPERTY()
	AActor *CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	UPROPERTY()
	class AAIController *EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor *PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent *PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AFWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 120.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.f;

public:
	AFEnemy();

	virtual void Tick(float DeltaTime) override;

	void CheckPatrolTarget();

	void CheckCombatTarget();

	virtual void GetHit_Implementation(const FVector &ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;

	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	virtual void Die() override;

	bool InTargetRange(AActor *Target, double Radius);

	void MoveToTarget(AActor *Target);

	AActor *ChoosePatrolTarget();

	UFUNCTION()
	void PawnSeen(APawn *SeenPawn);

	virtual void Attack() override;

	virtual bool CanAttack() override;

	virtual void HandleDamage(float DamageAmount) override;

	virtual int32 PlayDeathMontage() override;

private:
	void PatrolTimerFinished();

	void HideHealthBar();

	void ShowHealthBar();

	void LoseInterest();

	void StartPatrolling();

	void ChaseTarget();

	bool IsOutsideCombatRadius();

	bool IsOutsideAttackRadius();

	bool IsInsideAttackRadius();

	bool IsChasing();

	bool IsAttacking();

	bool IsDead();

	bool IsEngaged();

	void StartAttackTimer();

	void ClearAttackTimer();

	void ClearPatrolTimer();
};
