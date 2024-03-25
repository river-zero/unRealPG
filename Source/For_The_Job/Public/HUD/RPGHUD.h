#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RPGHUD.generated.h"

class URPGOverlay;

UCLASS()
class FOR_THE_JOB_API ARPGHUD : public AHUD {
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<URPGOverlay> RPGOverlayClass;

	UPROPERTY()
	URPGOverlay *RPGOverlay;

public:
	FORCEINLINE URPGOverlay *GetRPGOverlay() const { return RPGOverlay; }
};
