#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FItem.generated.h"

UCLASS()
class FOR_THE_JOB_API AFItem : public AActor {
	GENERATED_BODY()
	
public:	
	AFItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	float RunningTime;
	float Amplitude;
	float TimeConstant;
};
