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
	TArray<AActor*> IgnoreActors;

public:
	AFWeapon();

	void Equip(USceneComponent *InParent, FName InSocketName, AActor *NewOwner, APawn *NewInstigator);

	void DeactivateEmbers();

	void DisableSphereCollision();

	void PlayEquipSound();

	void AttachMeshToSocket(USceneComponent *InParent, const FName &InSocketName);

	FORCEINLINE UBoxComponent *GetWeaponBox() const { return WeaponBox; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	bool ActorIsSameType(AActor *OtherActor);

	void ExecuteGetHit(FHitResult &BoxHit);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector &FieldLocation);

private:
	void BoxTrace(FHitResult &BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = false;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase *EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent *WeaponBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent *BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent *BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;
};
