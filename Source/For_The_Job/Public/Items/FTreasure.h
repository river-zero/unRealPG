#pragma once

#include "CoreMinimal.h"
#include "Items/FItem.h"
#include "FTreasure.generated.h"

UCLASS()
class FOR_THE_JOB_API AFTreasure : public AFItem {
	GENERATED_BODY()

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase *PickupSound;
};
