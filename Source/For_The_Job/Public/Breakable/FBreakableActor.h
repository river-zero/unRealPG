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

	// �������Ʈ Ŭ������ C++���� ����ϱ� ����
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<class AFTreasure>> TreasureClasses;

	// ĳ���Ͱ� ��ü�� ������� ���ϵ��� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent *Capsule;

private:
	bool bBroken = false;
};
