#include "Components/FAttributeComponent.h"

UFAttributeComponent::UFAttributeComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UFAttributeComponent::BeginPlay() {
	Super::BeginPlay();
}

void UFAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
