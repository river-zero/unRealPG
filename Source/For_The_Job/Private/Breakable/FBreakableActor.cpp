#include "Breakable/FBreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

AFBreakableActor::AFBreakableActor() {
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject< UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void AFBreakableActor::BeginPlay() {
	Super::BeginPlay();
}

void AFBreakableActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AFBreakableActor::GetHit(const FVector &ImpactPoint) {

}
