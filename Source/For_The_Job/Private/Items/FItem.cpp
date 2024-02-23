#include "Items/FItem.h"
#include "For_The_Job/DebugMacros.h"

AFItem::AFItem() {
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
}

void AFItem::BeginPlay() {
	Super::BeginPlay();
}

float AFItem::TransformedSin() {
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AFItem::TransformedCos() {
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AFItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
}
