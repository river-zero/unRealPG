#include "Characters/FCharacter.h"

AFCharacter::AFCharacter() {
	PrimaryActorTick.bCanEverTick = true;
}

void AFCharacter::BeginPlay() {
	Super::BeginPlay();
}

void AFCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AFCharacter::MoveForward);
}

void AFCharacter::MoveForward(float Value) {
	if (Controller && (Value != 0.f)) {
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, Value);
	}
}
