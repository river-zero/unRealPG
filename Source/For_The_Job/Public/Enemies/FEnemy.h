#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/FHitInterface.h"
#include "Characters/CharacterType.h"
#include "FEnemy.generated.h"

class UAnimMontage;
class UFAttributeComponent;
class UFHealthBarComponent;

UCLASS()
class FOR_THE_JOB_API AFEnemy : public ACharacter, public IFHitInterface {
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

private:
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage *HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage *DieMontage;

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase *HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem *HitParticles;

	UPROPERTY(VisibleAnywhere)
	UFAttributeComponent *Attributes;

	UPROPERTY(VisibleAnywhere)
	UFHealthBarComponent *HealthBarWidget;

public:
	AFEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector &ImpactPoint) override;

	void DirectionalHitReact(const FVector &ImpactPoint);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	void Die();

	void PlayHitReactMontage(const FName &SelectionName);
};
