#include "Characters/RPGCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AttributeComponent.h"
#include "Items/Item.h"
#include "Items/Weapon.h"
#include "Animation/AnimMontage.h"
#include "HUD/RPGHUD.h"
#include "HUD/RPGOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"

ARPGCharacter::ARPGCharacter() {
    PrimaryActorTick.bCanEverTick = true;

    float CharacterHalfHeight = 90.f;
    float CharacterRadius = 40.f;
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
    GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void ARPGCharacter::Tick(float DeltaTime) {
    if (Attributes && RPGOverlay) {
        Attributes->RegenStamina(DeltaTime);
        RPGOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
    }
}

void ARPGCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ARPGCharacter::MoveForward);
    PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ARPGCharacter::MoveRight);
    PlayerInputComponent->BindAxis(FName("Turn"), this, &ARPGCharacter::Turn);
    PlayerInputComponent->BindAxis(FName("LookUp"), this, &ARPGCharacter::LookUp);

    PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ARPGCharacter::StartJump);
    PlayerInputComponent->BindAction(FName("Jump"), IE_Released, this, &ARPGCharacter::StopJump);
    PlayerInputComponent->BindAction(FName("WalkRun"), IE_Pressed, this, &ARPGCharacter::WalkRun);
    PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ARPGCharacter::EKeyPressed);
    PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ARPGCharacter::Attack);
    //PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &ARPGCharacter::Dodge);
}

void ARPGCharacter::Jump() {
    Super::Jump();
}

void ARPGCharacter::StartJump() {
    if (IsUnoccupied()) {
        bJumpInput = true;
        Jump();
    }
}

void ARPGCharacter::StopJump() {
    bJumpInput = false;
}

void ARPGCharacter::BeginPlay() {
    Super::BeginPlay();

    Tags.Add(FName("EngageableTarget"));
    InitializeRPGOverlay();
}

void ARPGCharacter::MoveForward(float Value) {
    if (ActionState != EActionState::EAS_Unoccupied) return;

    if (Controller && (Value != 0.f)) {
        const FRotator ControlRotation = GetControlRotation();
        const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ARPGCharacter::MoveRight(float Value) {
    if (ActionState != EActionState::EAS_Unoccupied) return;

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

void ARPGCharacter::EKeyPressed() {
    AWeapon *OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
    if (OverlappingWeapon) {
        if (EquippedWeapon) {
            EquippedWeapon->Destroy();
        }
        EquipWeapon(OverlappingWeapon);
    } else {
        if (CanDisarm()) {
            Disarm();
        } else if (CanArm()) {
            Arm();
        }
    }
}

void ARPGCharacter::Attack() {
    Super::Attack();

    if (CanAttack()) {
        PlayAttackMontage();
        ActionState = EActionState::EAS_Attacking;
    }
}

void ARPGCharacter::EquipWeapon(AWeapon *Weapon) {
    Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
    CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
    OverlappingItem = nullptr;
    EquippedWeapon = Weapon;
}

void ARPGCharacter::AttackEnd() {
    ActionState = EActionState::EAS_Unoccupied;
}

void ARPGCharacter::DodgeEnd() {
    Super::DodgeEnd();

    ActionState = EActionState::EAS_Unoccupied;
}

bool ARPGCharacter::CanAttack() {
    return ActionState == EActionState::EAS_Unoccupied &&
        CharacterState != ECharacterState::ECS_Unequipped;
}

bool ARPGCharacter::CanDisarm() {
    return ActionState == EActionState::EAS_Unoccupied && 
        CharacterState != ECharacterState::ECS_Unequipped;
}

bool ARPGCharacter::CanArm() {
    return ActionState == EActionState::EAS_Unoccupied &&
        CharacterState == ECharacterState::ECS_Unequipped &&
        EquippedWeapon;
}

void ARPGCharacter::Disarm() {
    PlayEquipMontage(FName("Unequip"));
    CharacterState = ECharacterState::ECS_Unequipped;
    ActionState = EActionState::EAS_EquippingWeapon;
}

void ARPGCharacter::Arm() {
    PlayEquipMontage(FName("Equip"));
    CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
    ActionState = EActionState::EAS_EquippingWeapon;
}

void ARPGCharacter::PlayEquipMontage(const FName &SectionName) {
    UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && EquipMontage) {
        AnimInstance->Montage_Play(EquipMontage);
        AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
    }
}

//void ARPGCharacter::Die_Implementation() {
//    Super::Die_Implementation();
//
//    ActionState = EActionState::EAS_Dead;
//    DisableMeshCollision();
//}

bool ARPGCharacter::HasEnoughStamina() {
    return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

bool ARPGCharacter::IsOccupied() {
    return ActionState != EActionState::EAS_Unoccupied;
}

void ARPGCharacter::AttachWeaponToBack() {
    if (EquippedWeapon) {
        EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
    }
}

void ARPGCharacter::AttachWeaponToHand() {
    if (EquippedWeapon) {
        EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
    }
}

void ARPGCharacter::FinishEquipping() {
    ActionState = EActionState::EAS_Unoccupied;
}

void ARPGCharacter::HitReactEnd() {
    ActionState = EActionState::EAS_Unoccupied;
}

bool ARPGCharacter::IsUnoccupied() {
    return ActionState == EActionState::EAS_Unoccupied;
}

void ARPGCharacter::InitializeRPGOverlay() {
    APlayerController *PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController) {
        ARPGHUD *RPGHUD = Cast<ARPGHUD>(PlayerController->GetHUD());
        if (RPGHUD) {
            RPGOverlay = RPGHUD->GetRPGOverlay();
            if (RPGOverlay && Attributes) {
                RPGOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
                RPGOverlay->SetStaminaBarPercent(1.f);
                RPGOverlay->SetGold(0);
                RPGOverlay->SetSouls(0);
            }
        }
    }
}

void ARPGCharacter::SetHUDHealth() {
    if (RPGOverlay && Attributes) {
        RPGOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
    }
}

void ARPGCharacter::WalkRun() {
    bIsRunning = !bIsRunning;

    if (bIsRunning) {
        GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
    } else {
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
}

float ARPGCharacter::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser) {
    HandleDamage(DamageAmount);
    SetHUDHealth();
    return DamageAmount;
}

void ARPGCharacter::GetHit_Implementation(const FVector &ImpactPoint, AActor *Hitter) {
    Super::GetHit_Implementation(ImpactPoint, Hitter);

    SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
    if (Attributes && Attributes->GetHealthPercent() > 0.f) {
        ActionState = EActionState::EAS_HitReaction;
    }
}

void ARPGCharacter::SetOverlappingItem(AItem *Item) {
    OverlappingItem = Item;
}

void ARPGCharacter::AddSouls(ASoul *Soul) {
    if (Attributes && RPGOverlay) {
        Attributes->AddSouls(Soul->GetSouls());
        RPGOverlay->SetSouls(Attributes->GetSouls());
    }
}

void ARPGCharacter::AddGold(ATreasure *Treasure) {
    if (Attributes && RPGOverlay) {
        Attributes->AddGold(Treasure->GetGold());
        RPGOverlay->SetGold(Attributes->GetGold());
    }
}
