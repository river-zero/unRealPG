#include "Pawns/FBird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AFBird::AFBird() {
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(GetRootComponent());

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

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

void AFBird::Turn(float Value) {
	AddControllerYawInput(Value);
}

void AFBird::LookUp(float Value) {
	AddControllerPitchInput(Value);
}

void AFBird::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AFBird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AFBird::MoveForward);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AFBird::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AFBird::LookUp);
}
