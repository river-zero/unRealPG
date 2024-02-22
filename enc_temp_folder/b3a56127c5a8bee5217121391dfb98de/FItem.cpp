#include "Items/FItem.h"
#include "For_The_Job/DebugMacros.h"

AFItem::AFItem() {
	PrimaryActorTick.bCanEverTick = true;

	Amplitude = 0.25f;
	TimeConstant = 5.f;
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

	// 위아래 움직임 구현
	RunningTime += DeltaTime;
	AddActorWorldOffset(FVector(TransformedSin(), TransformedCos(), 0.f));

	DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);
}
