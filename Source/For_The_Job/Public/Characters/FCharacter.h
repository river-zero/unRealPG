#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/FHitInterface.h"
#include "Characters/CharacterType.h"
#include "FCharacter.generated.h"

class AFWeapon;
class UFAttributeComponent;
class UAnimMontage;

UCLASS()
class FOR_THE_JOB_API AFCharacter : public ACharacter, public IFHitInterface {
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase *HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem *HitParticles;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;

protected:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AFWeapon *EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage *AttackAnimation;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage *HitReactMontage;

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

	virtual int32 PlayAttackAnimation();

	virtual int32 PlayDeathMontage();

	void PlayHitReactMontage(const FName &SelectionName);

	void DirectionalHitReact(const FVector &ImpactPoint);

	virtual bool CanAttack();

	bool IsAlive();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	void PlayHitSound(const FVector &ImpactPoint);

	void SpawnHitParticles(const FVector &ImpactPoint);

	virtual void HandleDamage(float DamageAmount);

	void DisableCapsule();

private:
	void PlayMontageSection(UAnimMontage *Montage, const FName &SectionName);

	int32 PlayRandomMontageSection(UAnimMontage *Montage, const TArray<FName> &SectionNames);
};
