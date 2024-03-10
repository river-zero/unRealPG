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

	virtual void GetHit(const FVector &ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent *GeometryCollection;
};
