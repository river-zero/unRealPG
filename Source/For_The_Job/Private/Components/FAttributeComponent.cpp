#include "Components/FAttributeComponent.h"

void UFAttributeComponent::ReceiveDamage(float Damage) {
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

float UFAttributeComponent::GetHealthPercent() {
	return Health / MaxHealth;
}

bool UFAttributeComponent::IsAlive() {
	return Health > 0.f;
}

UFAttributeComponent::UFAttributeComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UFAttributeComponent::BeginPlay() {
	Super::BeginPlay();
}

void UFAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
