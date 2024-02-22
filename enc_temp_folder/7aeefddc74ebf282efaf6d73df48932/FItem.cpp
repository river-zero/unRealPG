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

void AFItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// 위아래로 움직임 구현
	RunningTime += DeltaTime;
	float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);
	AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ));

	DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);
}
