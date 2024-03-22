#include "Characters/RPGCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"

ARPGCharacter::ARPGCharacter() {
    PrimaryActorTick.bCanEverTick = true;

    float CharacterHalfHeight = 90.f;
    float CharacterRadius = 30.f;
    GetCapsuleComponent()->InitCapsuleSize(CharacterRadius, CharacterHalfHeight);
    FVector PivotPosition(0.f, 0.f, -CharacterHalfHeight);
    FRotator PivotRotation(0.f, -90.f, 0.f);
    GetMesh()->SetRelativeLocationAndRotation(PivotPosition, PivotRotation);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
    GetMesh()->SetGenerateOverlapEvents(true);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 300.f;

    ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
    ViewCamera->SetupAttachment(CameraBoom);

    GetCharacterMovement()->GravityScale = 3.f;
    GetCharacterMovement()->JumpZVelocity = 1200.f;
}

void ARPGCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

void ARPGCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ARPGCharacter::MoveForward);
    PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ARPGCharacter::MoveRight);
    PlayerInputComponent->BindAxis(FName("Turn"), this, &ARPGCharacter::Turn);
    PlayerInputComponent->BindAxis(FName("LookUp"), this, &ARPGCharacter::LookUp);

    PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ARPGCharacter::StartJump);
    PlayerInputComponent->BindAction(FName("Jump"), IE_Released, this, &ARPGCharacter::StopJump);
}

void ARPGCharacter::Jump() {
    Super::Jump();
}

void ARPGCharacter::StartJump() {
    bJumpInput = true;
    Jump();
}

void ARPGCharacter::StopJump() {
    bJumpInput = false;
}

void ARPGCharacter::BeginPlay() {
    Super::BeginPlay();
}

void ARPGCharacter::MoveForward(float Value) {
    if (Controller && (Value != 0.f)) {
        const FRotator ControlRotation = GetControlRotation();
        const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ARPGCharacter::MoveRight(float Value) {
    if (Controller && (Value != 0.f)) {
        const FRotator ControlRotation = GetControlRotation();
        const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void ARPGCharacter::Turn(float Value) {
    AddControllerYawInput(Value);
}

void ARPGCharacter::LookUp(float Value) {
    AddControllerPitchInput(Value);
}
