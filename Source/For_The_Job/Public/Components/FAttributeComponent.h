#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FAttributeComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FOR_THE_JOB_API UFAttributeComponent : public UActorComponent {
	GENERATED_BODY()

public:
	void ReceiveDamage(float Damage);

	float GetHealthPercent();

private:
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;

public:	
	UFAttributeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
};
