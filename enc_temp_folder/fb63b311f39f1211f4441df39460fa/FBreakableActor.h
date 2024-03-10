#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FBreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class FOR_THE_JOB_API AFBreakableActor : public AActor {
	GENERATED_BODY()
	
public:	
	AFBreakableActor();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent *GeometryCollection;
};
