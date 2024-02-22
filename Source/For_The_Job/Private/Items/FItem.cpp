#include "Items/FItem.h"
#include "DrawDebugHelpers.h"
#include "For_The_Job/For_The_Job.h"

AFItem::AFItem() {
	PrimaryActorTick.bCanEverTick = true;
}

void AFItem::BeginPlay() {
	Super::BeginPlay();

	UWorld* World = GetWorld();
	FVector Location = GetActorLocation();
	DRAW_SPHERE(Location)
}

void AFItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
