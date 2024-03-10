#include "Breakable/FBreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

AFBreakableActor::AFBreakableActor() {
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject< UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
}

void AFBreakableActor::BeginPlay() {
	Super::BeginPlay();
}

void AFBreakableActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
