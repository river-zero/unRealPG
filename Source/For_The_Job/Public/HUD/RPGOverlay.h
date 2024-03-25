#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGOverlay.generated.h"

UCLASS()
class FOR_THE_JOB_API URPGOverlay : public UUserWidget {
	GENERATED_BODY()
	
public:
	void SetHealthBarPercent(float Percent);
	
	void SetStaminaBarPercent(float Percent);
	
	void SetGold(int32 Gold);
	
	void SetSouls(int32 Souls);

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar *HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar *StaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *GoldText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *SoulsText;
};
