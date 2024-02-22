#include "Items/FItem.h"
#include "For_The_Job/DebugMacros.h"

AFItem::AFItem() {
	PrimaryActorTick.bCanEverTick = true;
}

void AFItem::BeginPlay() {
	Super::BeginPlay();

	UWorld* World = GetWorld();

	// SetActorLocation(FVector(0.f, 0.f, 50.f));
	// SetActorRotation(FRotator(0.f, 45.f, 0.f));
	FVector Location = GetActorLocation();
	FVector Forward = GetActorForwardVector();

	DRAW_SPHERE(Location);
	DRAW_VECTOR(Location, Location + Forward * 100.f)
}

void AFItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
