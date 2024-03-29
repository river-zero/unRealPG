# unRealRPG
해당 프로젝트는 Unreal Engine 5.1 버전을 사용해 구현한 오픈 월드 방식의 액션 RPG 게임입니다.

# 주요 기능
## 플레이어
### 이동

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
### Inverse Kinematics를 이용한 발 위치 조정
### 점프
### 시점 변화 (백뷰, 쿼터뷰)

## 전투
### 무장/비무장 상태 전환
### 공격
### 방향에 따른 공격 반응 (히트 리액션)
### 회피
### 죽음

## 아이템
### 부서지는 오브젝트 (항아리)
### 무기
### 보물 (금화)
### 소울

## 적
### 순찰
### 플레이어 감지 및 추적
### 모션 워핑을 이용한 공격

## 화면
### 체력바
### 스테미너바
### 금화 표시
### 소울 표시

# 시행 착오