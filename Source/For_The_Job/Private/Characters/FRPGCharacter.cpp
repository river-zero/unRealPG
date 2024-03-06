#include "Characters/FRPGCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inputs/FInputConfigData.h"
#include "Items/FItem.h"
#include "Items/Weapons/FWeapon.h"
#include "Animations/FAnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"

AFRPGCharacter::AFRPGCharacter() {
	PrimaryActorTick.bCanEverTick = true;
}

void AFRPGCharacter::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);

    switch (CurrentViewMode) {
    case EViewMode::BackView:
        break;
    case EViewMode::QuarterView: {
        if (KINDA_SMALL_NUMBER < DirectionToMove.SizeSquared()) {
            GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
            AddMovementInput(DirectionToMove);
            DirectionToMove = FVector::ZeroVector;
        }

        break;
    }
    case EViewMode::End:
        break;
    default:
        break;
    }

    // 시점 전환 시 부드러운 이동 위함
    if (KINDA_SMALL_NUMBER < abs(DestArmLength - CameraBoom->TargetArmLength)) {
        CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, DestArmLength, DeltaSeconds, ArmLengthChangeSpeed);
        CameraBoom->SetRelativeRotation(FMath::RInterpTo(CameraBoom->GetRelativeRotation(), DestArmRotation, DeltaSeconds, ArmRotationChangeSpeed));
    }
}

void AFRPGCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (true == ::IsValid(EnhancedInputComponent)) {
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->MoveAction, ETriggerEvent::Triggered, this, &AFRPGCharacter::Move);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->LookAction, ETriggerEvent::Triggered, this, &AFRPGCharacter::Look);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->ChangeViewAction, ETriggerEvent::Started, this, &AFRPGCharacter::ChangeView);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->RunAction, ETriggerEvent::Started, this, &AFRPGCharacter::Run);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->JumpAction, ETriggerEvent::Started, this, &AFRPGCharacter::StartJump);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->JumpAction, ETriggerEvent::Canceled, this, &AFRPGCharacter::StopJump);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->InteractAction, ETriggerEvent::Started, this, &AFRPGCharacter::EKeyPressed);
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->AttackAction, ETriggerEvent::Started, this, &AFRPGCharacter::Attack);
    }
}

void AFRPGCharacter::PossessedBy(AController *NewController) {
    Super::PossessedBy(NewController);

    // SetViewMode(EViewMode::QuarterView);
    SetViewMode(EViewMode::BackView);
    DestArmLength = 400.f;
    DestArmRotation = FRotator::ZeroRotator;
}

void AFRPGCharacter::SetViewMode(EViewMode InViewMode) {
    if (CurrentViewMode == InViewMode) {
        return;
    }

    CurrentViewMode = InViewMode;

    switch (CurrentViewMode) {
    case EViewMode::BackView:
        // CameraBoom->TargetArmLength = 400.f;
        // CameraBoom->SetRelativeRotation(FRotator::ZeroRotator);

        bUseControllerRotationPitch = false;
        bUseControllerRotationYaw = false;
        bUseControllerRotationRoll = false;

        CameraBoom->bUsePawnControlRotation = true;
        CameraBoom->bDoCollisionTest = true;
        CameraBoom->bInheritPitch = true;
        CameraBoom->bInheritYaw = true;
        CameraBoom->bInheritRoll = false;

        GetCharacterMovement()->bOrientRotationToMovement = true;
        GetCharacterMovement()->bUseControllerDesiredRotation = false;
        GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

        break;
    case EViewMode::QuarterView:
        // CameraBoom->TargetArmLength = 900.f;
        // CameraBoom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

        bUseControllerRotationPitch = false;
        bUseControllerRotationYaw = false;
        bUseControllerRotationRoll = false;

        CameraBoom->bUsePawnControlRotation = false;
        CameraBoom->bDoCollisionTest = false;
        CameraBoom->bInheritPitch = false;
        CameraBoom->bInheritYaw = false;
        CameraBoom->bInheritRoll = false;

        GetCharacterMovement()->bOrientRotationToMovement = false;
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

        break;
    case EViewMode::End:
        break;
    default:
        break;
    }
}

void AFRPGCharacter::OnAttackMontageEnded(UAnimMontage *Montage, bool bInterrupted) {
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    EActionState::EAS_Unoccupied;
}

void AFRPGCharacter::BeginPlay() {
	Super::BeginPlay();

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (true == ::IsValid(PlayerController)) {
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (true == ::IsValid(Subsystem)) {
            Subsystem->AddMappingContext(PlayerCharacterInputMappingContext, 0);
        }
    }

    UFAnimInstance *AnimInstance = Cast<UFAnimInstance>(GetMesh()->GetAnimInstance());
    if (true == ::IsValid(AnimInstance)) {
        AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnAttackMontageEnded);
        AnimInstance->OnCheckHitDelegate.AddDynamic(this, &ThisClass::CheckHit);
        AnimInstance->OnAttackEndDelegate.AddDynamic(this, &ThisClass::CheckCanNextCombo);
    }
}

void AFRPGCharacter::Move(const FInputActionValue &InValue) {
    // 캐릭터가 공격 중인 경우 이동 제한을 위함
    // if (ActionState == EActionState::EAS_Attacking) return;

    FVector2D MovementVector = InValue.Get<FVector2D>();

    switch (CurrentViewMode) {
    case EViewMode::None:
        break;
    case EViewMode::BackView: {
        const FRotator ControlRotation = GetController()->GetControlRotation();
        const FRotator ControlRotationYaw(0.f, ControlRotation.Yaw, 0.f);

        const FVector ForwardVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::X);
        const FVector RightVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardVector, MovementVector.X);
        AddMovementInput(RightVector, MovementVector.Y);

        break;
    }
    case EViewMode::QuarterView:
        DirectionToMove.X = MovementVector.X;
        DirectionToMove.Y = MovementVector.Y;

        break;
    case EViewMode::End:
        break;
    default:
        AddMovementInput(GetActorForwardVector(), MovementVector.X);
        AddMovementInput(GetActorRightVector(), MovementVector.Y);

        break;
    }
}

void AFRPGCharacter::Look(const FInputActionValue &InValue) {
    FVector2D LookVector = InValue.Get<FVector2D>();

    switch (CurrentViewMode) {
    case EViewMode::None:
        break;
    case EViewMode::BackView:
        AddControllerYawInput(LookVector.X);
        AddControllerPitchInput(LookVector.Y);

        break;
    case EViewMode::QuarterView:
        break;
    case EViewMode::End:
        break;
    default:
        break;
    }
}

void AFRPGCharacter::ChangeView(const FInputActionValue &InValue) {
    switch (CurrentViewMode) {
    case EViewMode::BackView:
        GetController()->SetControlRotation(GetActorRotation());
        DestArmLength = 900.f;
        DestArmRotation = FRotator(-45.f, 0.f, 0.f);
        SetViewMode(EViewMode::QuarterView);

        break;
    case EViewMode::QuarterView:
        GetController()->SetControlRotation(FRotator::ZeroRotator);
        DestArmLength = 400.f;
        DestArmRotation = FRotator::ZeroRotator;
        SetViewMode(EViewMode::BackView);

        break;
    case EViewMode::End:
        break;
    default:
        break;
    }
}

void AFRPGCharacter::Run(const FInputActionValue &InValue) {
    IsWalking = !IsWalking;

    if (!IsWalking) {
        GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
    } else {
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
}

void AFRPGCharacter::Jump() {
    Super::Jump();

    GetCharacterMovement()->GravityScale = GravityScaleOnJumpStart;
}

void AFRPGCharacter::StartJump() {
    Jump();
}

void AFRPGCharacter::StopJump() {
    GetCharacterMovement()->GravityScale = DefaultGravityScale;
}

void AFRPGCharacter::EKeyPressed() {
    AFWeapon *OverlappingWeapon = Cast< AFWeapon>(OverlappingItem);
    if (OverlappingWeapon) {
        OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
        CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
    }
}

void AFRPGCharacter::Attack() {
    if (0 == CurrentComboCount) {
        BeginCombo();
        return;
    } else {
        ensure(FMath::IsWithinInclusive<int32>(CurrentComboCount, 1, MaxComboCount));
        bIsAttackKeyPressed = true;
    }
}

bool AFRPGCharacter::CanAttack() {
    return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

void AFRPGCharacter::CheckHit() {
    UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("CheckHit() has been called.")));
}

void AFRPGCharacter::BeginCombo() {
    UFAnimInstance *AnimInstance = Cast<UFAnimInstance>(GetMesh()->GetAnimInstance());
    if (false == ::IsValid(AnimInstance)) {
        return;
    }

    if (CanAttack()) {
        CurrentComboCount = 1;

        // 공격하는 동안에는 이동하지 못하게 함
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

        ActionState = EActionState::EAS_Attacking;

        AnimInstance->PlayAttackMontage();

        FOnMontageEnded OnMontageEndedDelegate;
        OnMontageEndedDelegate.BindUObject(this, &ThisClass::EndCombo);
        AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, AnimInstance->AttackMontage);
    }

}

void AFRPGCharacter::CheckCanNextCombo() {
    UFAnimInstance *AnimInstance = Cast<UFAnimInstance>(GetMesh()->GetAnimInstance());
    if (false == ::IsValid(AnimInstance)) {
        return;
    }

    if (true == bIsAttackKeyPressed) {
        CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, MaxComboCount);

        FName NextSectionName = *FString::Printf(TEXT("%s%d"), *AttackAnimMontageSectionName, CurrentComboCount);
        AnimInstance->Montage_JumpToSection(NextSectionName, AnimInstance->AttackMontage);
        bIsAttackKeyPressed = false;
    }
}

void AFRPGCharacter::EndCombo(UAnimMontage *InAnimMontage, bool bInterrupted) {
    ensure(0 != CurrentComboCount);
    CurrentComboCount = 0;
    bIsAttackKeyPressed = false;
    ActionState = EActionState::EAS_Unoccupied;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
