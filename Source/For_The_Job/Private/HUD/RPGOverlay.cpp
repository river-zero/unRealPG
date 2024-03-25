#include "HUD/RPGOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void URPGOverlay::SetHealthBarPercent(float Percent) {
	if (HealthProgressBar) {
		HealthProgressBar->SetPercent(Percent);
	}
}

void URPGOverlay::SetStaminaBarPercent(float Percent) {
	if (StaminaProgressBar) {
		StaminaProgressBar->SetPercent(Percent);
	}
}

void URPGOverlay::SetGold(int32 Gold) {
	if (GoldText) {
		const FString String = FString::Printf(TEXT("%d"), Gold);
		const FText Text = FText::FromString(String);
		GoldText->SetText(Text);
	}
}

void URPGOverlay::SetSouls(int32 Souls) {
	if (SoulsText) {
		const FString String = FString::Printf(TEXT("%d"), Souls);
		const FText Text = FText::FromString(String);
		SoulsText->SetText(Text);
	}
}
