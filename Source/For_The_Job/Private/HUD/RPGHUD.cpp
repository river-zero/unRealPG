#include "HUD/RPGHUD.h"
#include "HUD/RPGOverlay.h"

void ARPGHUD::BeginPlay() {
	Super::BeginPlay();

	UWorld *World = GetWorld();
	if (World) {
		APlayerController *Controller = World->GetFirstPlayerController();
		if (Controller && RPGOverlayClass) {
			RPGOverlay = CreateWidget<URPGOverlay>(Controller, RPGOverlayClass);
			RPGOverlay->AddToViewport();
		}
	}
}
