#pragma once

#include "CoreMinimal.h"
#include "Items/FItem.h"
#include "FWeapon.generated.h"

class USoundBase;
class UBoxComponent;

UCLASS()
class FOR_THE_JOB_API AFWeapon : public AFItem {
	GENERATED_BODY()

public:
	AFWeapon();

	void Equip(USceneComponent *InParent, FName InSocketName);

	void AttachMeshToSocket(USceneComponent *InParent, const FName &InSocketName);
	
protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

private:
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase *EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent *WeaponBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent *BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent *BoxTraceEnd;
};
