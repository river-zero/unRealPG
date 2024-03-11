#include "UI/FHealthBarComponent.h"
#include "UI/FHealthBar.h"
#include "Components/ProgressBar.h"

void UFHealthBarComponent::SetHealthPercent(float Percent) {
	if (HealthBarWidget == nullptr) {
		HealthBarWidget = Cast<UFHealthBar>(GetUserWidgetObject());
	}
	if (HealthBarWidget && HealthBarWidget->HealthBar) {
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
