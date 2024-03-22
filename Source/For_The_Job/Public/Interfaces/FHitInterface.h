#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FHitInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFHitInterface : public UInterface {
	GENERATED_BODY()
};

class FOR_THE_JOB_API IFHitInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector &ImpactPoint, AActor *Hitter);
};
