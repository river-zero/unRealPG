- [unRealRPG](#unrealrpg)
- [플레이 영상](#플레이-영상)
- [주요 기능](#주요-기능)
  - [플레이어](#플레이어)
    - [이동](#이동)
    - [걷기/달리기](#걷기달리기)
    - [Inverse Kinematics를 이용한 발 위치 조정](#inverse-kinematics를-이용한-발-위치-조정)
    - [점프](#점프)
    - [시점 변화 (백뷰, 쿼터뷰)](#시점-변화-백뷰-쿼터뷰)
  - [전투](#전투)
    - [무장/비무장 상태 전환](#무장비무장-상태-전환)
    - [공격](#공격)
    - [방향에 따른 공격 반응 (히트 리액션)](#방향에-따른-공격-반응-히트-리액션)
    - [회피](#회피)
    - [죽음](#죽음)
  - [아이템](#아이템)
    - [부서지는 오브젝트 (항아리)](#부서지는-오브젝트-항아리)
    - [무기](#무기)
    - [보물 (금화)](#보물-금화)
    - [소울](#소울)
  - [적](#적)
    - [순찰](#순찰)
    - [플레이어 감지 및 추적](#플레이어-감지-및-추적)
    - [모션 워핑을 이용한 공격](#모션-워핑을-이용한-공격)
  - [화면](#화면)
    - [체력바](#체력바)
    - [스테미너바](#스테미너바)
    - [금화 표시](#금화-표시)
    - [소울 표시](#소울-표시)
- [시행 착오](#시행-착오)

# unRealRPG
해당 프로젝트는 Unreal Engine 5.1 버전을 사용해 구현한 오픈 월드 방식의 액션 RPG 게임입니다.

# 플레이 영상
https://github.com/river-zero/unRealPG/assets/124229424/21bb3c5b-a1c2-455c-9a33-5e78b83c2c4b

# 주요 기능
## 플레이어
### 이동
![](/README/move.gif)

![](/README/8.png)
![](/README/9.png)
![](/README/10.png)
![](/README/11.png)

```
void ARPGCharacter::MoveForward(float Value) {
    if (ActionState != EActionState::EAS_Unoccupied) return;

    if (Controller && (Value != 0.f)) {
        switch (CurrentViewMode) {
        case EViewMode::None:
            break;
        case EViewMode::BackView:
        {
            const FRotator ControlRotation = GetController()->GetControlRotation();
            const FRotator ControlRotationYaw(0.f, ControlRotation.Yaw, 0.f);

            const FVector ForwardVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::X);
            AddMovementInput(ForwardVector, Value);

            break;
        }
        case EViewMode::QuarterView:
            DirectionToMove.X = Value;
            break;
        case EViewMode::End:
            break;
        default:
            AddMovementInput(GetActorForwardVector(), Value);
            break;
        }
    }
}

void ARPGCharacter::MoveRight(float Value) {
    if (ActionState != EActionState::EAS_Unoccupied) return;

    if (Controller && (Value != 0.f)) {
        switch (CurrentViewMode) {
        case EViewMode::None:
            break;
        case EViewMode::BackView:
        {
            const FRotator ControlRotation = GetController()->GetControlRotation();
            const FRotator ControlRotationYaw(0.f, ControlRotation.Yaw, 0.f);

            const FVector RightVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::Y);
            AddMovementInput(RightVector, Value);

            break;
        }
        case EViewMode::QuarterView:
            DirectionToMove.Y = Value;
            break;
        case EViewMode::End:
            break;
        default:
            AddMovementInput(GetActorRightVector(), Value);
            break;
        }
    }
}

void ARPGCharacter::Turn(float Value) {
    switch (CurrentViewMode) {
    case EViewMode::None:
        break;
    case EViewMode::BackView:
        AddControllerYawInput(Value);
        break;
    case EViewMode::QuarterView:
        break;
    case EViewMode::End:
        break;
    default:
        break;
    }
}

void ARPGCharacter::LookUp(float Value) {
    switch (CurrentViewMode) {
    case EViewMode::None:
        break;
    case EViewMode::BackView:
        AddControllerPitchInput(Value);
        break;
    case EViewMode::QuarterView:
        break;
    case EViewMode::End:
        break;
    default:
        break;
    }
}
```

### 걷기/달리기
![](/README/walkrun.gif)

![](/README/12.png)

```
void ARPGCharacter::WalkRun() {
    bIsRunning = !bIsRunning;

    if (bIsRunning) {
        GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
    } else {
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
}
```

### Inverse Kinematics를 이용한 발 위치 조정
![](/README/inversekinematics.gif)

![](/README/1.png)
![](/README/2.png)
![](/README/3.png)
![](/README/4.png)
![](/README/5.png)
![](/README/6.png)
![](/README/7.png)

### 점프
![](/README/jump.gif)

![](/README/13.png)

```
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
```

### 시점 변화 (백뷰, 쿼터뷰)
![](/README/changeview.gif)

```
void ARPGCharacter::ChangeView() {
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

void ARPGCharacter::SetViewMode(EViewMode InViewMode) {
    if (CurrentViewMode == InViewMode) {
        return;
    }

    CurrentViewMode = InViewMode;

    switch (CurrentViewMode) {
    case EViewMode::BackView:
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
```

## 전투
### 무장/비무장 상태 전환
![](/README/armunarm.gif)

```
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
```

### 공격
![](/README/attack.gif)

```
void ARPGCharacter::Attack() {
    Super::Attack();

    if (CanAttack()) {
        PlayAttackMontage();
        ActionState = EActionState::EAS_Attacking;
    }
}
```
### 방향에 따른 공격 반응 (히트 리액션)
![](/README/hitreact.gif)

### 회피
![](/README/dodge.gif)

```
void ARPGCharacter::Dodge() {
    if (IsOccupied() || !HasEnoughStamina()) return;

    PlayDodgeMontage();
    ActionState = EActionState::EAS_Dodge;
    if (Attributes && RPGOverlay) {
        Attributes->UseStamina(Attributes->GetDodgeCost());
        RPGOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
    }
}
```

### 죽음
![](/README/death.gif)

![](/README/14.png)

```
void ARPGCharacter::Die_Implementation() {
    Super::Die_Implementation();

    ActionState = EActionState::EAS_Dead;
    DisableMeshCollision();
}
```

## 아이템
### 부서지는 오브젝트 (항아리)
![](/README/breakable.gif)

### 무기

### 보물 (금화)
![](/README/treasure.gif)

### 소울
![](/README/soul.gif)

## 적
### 순찰
![](/README/patrol.gif)

### 플레이어 감지 및 추적
![](/README/chase.gif)

### 모션 워핑을 이용한 공격
![](/README/motionwarpoing.gif)

## 화면
### 체력바
### 스테미너바
### 금화 표시
### 소울 표시

# 시행 착오
