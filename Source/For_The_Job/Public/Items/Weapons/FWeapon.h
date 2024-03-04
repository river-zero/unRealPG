#pragma once

#include "CoreMinimal.h"
#include "Items/FItem.h"
#include "FWeapon.generated.h"

UCLASS()
class FOR_THE_JOB_API AFWeapon : public AFItem {
	GENERATED_BODY()

public:
	void Equip(USceneComponent *InParent, FName InSocketName);
	
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) override;
};
