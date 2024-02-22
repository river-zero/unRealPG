#include "Items/FItem.h"
#include "For_The_Job/DebugMacros.h"

AFItem::AFItem() {
	PrimaryActorTick.bCanEverTick = true;
}

void AFItem::BeginPlay() {
	Super::BeginPlay();

	UWorld* World = GetWorld();
	FVector Location = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	DRAW_SPHERE(Location);
	DRAW_VECTOR(Location, Location + Forward * 100.f)
}

void AFItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
