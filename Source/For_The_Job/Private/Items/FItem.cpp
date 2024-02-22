#include "Items/FItem.h"
#include "For_The_Job/DebugMacros.h"

AFItem::AFItem() {
	PrimaryActorTick.bCanEverTick = true;
}

void AFItem::BeginPlay() {
	Super::BeginPlay();
}

void AFItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	float MovementRate = 60.f;
	float RotationRate = 45.f;

	AddActorWorldOffset(FVector(MovementRate * DeltaTime, 0.f, 0.f));
	AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));
	DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);
}
