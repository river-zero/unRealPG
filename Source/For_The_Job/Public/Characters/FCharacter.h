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

public:
	AFCharacter();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void Attack();

	virtual void Die();

	void DirectionalHitReact(const FVector &ImpactPoint);

	virtual void HandleDamage(float DamageAmount);

	void PlayHitSound(const FVector &ImpactPoint);

	void SpawnHitParticles(const FVector &ImpactPoint);

	void DisableCapsule();

	bool IsAlive();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void EnableBoxCollision();

	UFUNCTION(BlueprintCallable)
	void DisableBoxCollision();

	void PlayHitReactMontage(const FName &SelectionName);

	virtual int32 PlayAttackAnimation();

	virtual int32 PlayDeathMontage();

	virtual bool CanAttack();

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AFWeapon *EquippedWeapon;

	UPROPERTY(VisibleAnywhere)
	UFAttributeComponent *Attributes;

private:
	void PlayMontageSection(UAnimMontage *Montage, const FName &SectionName);

	int32 PlayRandomMontageSection(UAnimMontage *Montage, const TArray<FName> &SectionNames);

	UPROPERTY(EditAnywhere, Category = Combat)
	USoundBase *HitSound;

	UPROPERTY(EditAnywhere, Category = Combat)
	UParticleSystem *HitParticles;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage *AttackAnimation;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage *HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage *DieMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;
};
