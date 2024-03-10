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

	virtual void GetHit_Implementation(const FVector &ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent *GeometryCollection;

	// 블루프린트 클래스를 C++에서 사용하기 위함
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<class AFTreasure>> TreasureClasses;

	// 캐릭터가 물체를 통과하지 못하도록 함
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent *Capsule;

private:
	bool bBroken = false;
};
