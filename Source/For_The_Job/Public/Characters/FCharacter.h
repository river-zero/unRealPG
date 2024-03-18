#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/FHitInterface.h"
#include "FCharacter.generated.h"

class AFWeapon;
class UFAttributeComponent;
class UAnimMontage;

UCLASS()
class FOR_THE_JOB_API AFCharacter : public ACharacter, public IFHitInterface {
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AFWeapon *EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage *AttackAnimation;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage *HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase *HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem *HitParticles;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage *DieMontage;

	UPROPERTY(VisibleAnywhere)
	UFAttributeComponent *Attributes;

public:
	UFUNCTION(BlueprintCallable)
	void EnableBoxCollision();

	UFUNCTION(BlueprintCallable)
	void DisableBoxCollision();

public:
	AFCharacter();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void Attack();

	virtual void Die();

	virtual void PlayAttackAnimation();

	void PlayHitReactMontage(const FName &SelectionName);

	void DirectionalHitReact(const FVector &ImpactPoint);

	virtual bool CanAttack();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
};
