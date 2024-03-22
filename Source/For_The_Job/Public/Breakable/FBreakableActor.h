#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/FHitInterface.h"
#include "FBreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class FOR_THE_JOB_API AFBreakableActor : public AActor, public  IFHitInterface {
	GENERATED_BODY()
	
public:	
	AFBreakableActor();

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector &ImpactPoint, AActor *Hitter) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent *GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent *Capsule;

private:
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<class AFTreasure>> TreasureClasses;

	bool bBroken = false;
};
