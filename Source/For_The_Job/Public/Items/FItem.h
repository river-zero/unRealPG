#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FItem.generated.h"

UCLASS()
class FOR_THE_JOB_API AFItem : public AActor {
	GENERATED_BODY()
	
public:	
	AFItem();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransformedCos();

	template<typename T>
	T Avg(T First, T Second);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float RunningTime;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float TimeConstant = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;
};

template<typename T>
inline T AFItem::Avg(T First, T Second) {
	return (First + Second) / 2;
}
