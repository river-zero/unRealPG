#include "Breakable/FBreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/FTreasure.h"
#include "Components/CapsuleComponent.h"

AFBreakableActor::AFBreakableActor() {
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject< UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void AFBreakableActor::BeginPlay() {
	Super::BeginPlay();
}

void AFBreakableActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AFBreakableActor::GetHit_Implementation(const FVector &ImpactPoint) {
	UWorld *World = GetWorld();
	if (World && TreasureClass) {
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		World->SpawnActor<AFTreasure>(TreasureClass, Location, GetActorRotation());
	}
}
