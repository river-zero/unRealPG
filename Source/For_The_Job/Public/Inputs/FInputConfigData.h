#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FInputConfigData.generated.h"

UCLASS()
class FOR_THE_JOB_API UFInputConfigData : public UDataAsset {
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInputAction> MoveAction;
};
