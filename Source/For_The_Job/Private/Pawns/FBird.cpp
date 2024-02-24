#include "Pawns/FBird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

AFBird::AFBird() {
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(GetRootComponent());

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AFBird::BeginPlay() {
	Super::BeginPlay();
}

void AFBird::MoveForward(float Value) {
	if (Controller && (Value != 0.f)) {
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, Value);
	}
}

void AFBird::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AFBird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AFBird::MoveForward);
}
