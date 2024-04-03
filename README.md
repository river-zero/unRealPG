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

void ARPGCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    switch (CurrentViewMode) {
    case EViewMode::BackView:
        break;
    case EViewMode::QuarterView:
    {
        // 캐릭터의 이동 방향과 카메라의 회전을 동기화하여
        // 캐릭터가 이동하는 방향으로 카메라가 자연스럽게 회전
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

    // 카메라 거리와 회전을 부드럽게 변경하기 위한 보간 작업
    if (KINDA_SMALL_NUMBER < abs(DestArmLength - CameraBoom->TargetArmLength)) {
        CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, DestArmLength, DeltaTime, ArmLengthChangeSpeed);
        CameraBoom->SetRelativeRotation(FMath::RInterpTo(CameraBoom->GetRelativeRotation(), DestArmRotation, DeltaTime, ArmRotationChangeSpeed));
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
```

### 공격
![](/README/attack.gif)

```
void ABaseCharacter::Attack() {
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead"))) {
		CombatTarget = nullptr;
	}
}
```

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

```
void ABaseCharacter::GetHit_Implementation(const FVector &ImpactPoint, AActor *Hitter) {
	if (IsAlive() && Hitter) {
		DirectionalHitReact(Hitter->GetActorLocation());
	} else Die();

	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

void ABaseCharacter::DirectionalHitReact(const FVector &ImpactPoint) {
    // 캐릭터의 전방 벡터와 피격 지점의 방향 벡터를 계산
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

    // 외적을 사용하여 피격 방향이 왼쪽인지 오른쪽인지 판별
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0) {
		Theta *= -1.f;
	}

    // 각도 범위에 따라 적절한 히트 리액션 섹션을 선택
	FName Section("FromBack");
	if (Theta >= -45.f && Theta < 45.f) {
		Section = FName("FromFront");
	} else if (Theta >= -135.f && Theta < -45.f) {
		Section = FName("FromLeft");
	} else if (Theta >= 45.f && Theta < 135.f) {
		Section = FName("FromRight");
	}
	PlayHitReactMontage(Section);
}

void ABaseCharacter::PlayHitSound(const FVector &ImpactPoint) {
	if (HitSound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector &ImpactPoint) {
	if (HitParticles && GetWorld()) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}
```

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
void ABaseCharacter::Die_Implementation() {
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
}

int32 ABaseCharacter::PlayDeathMontage() {
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX) {
		DeathPose = Pose;
	}

	return Selection;
}
```

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

```
class FOR_THE_JOB_API ABreakableActor : public AActor, public IHitInterface {
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent *GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent *Capsule;

private:
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;

	bool bBroken = false;
};
```

```
void ABreakableActor::GetHit_Implementation(const FVector &ImpactPoint, AActor *Hitter) {
	if (bBroken) return;
	bBroken = true;

	UWorld *World = GetWorld();
	if (World && TreasureClasses.Num() > 0) {
		FVector Location = GetActorLocation();
		Location.Z += 75.f;

		const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);
		World->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
	}
}
```

### 무기
```
void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled) {
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox()) {
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}
```

```
void ARPGCharacter::EquipWeapon(AWeapon *Weapon) {
    Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
    CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
    OverlappingItem = nullptr;
    EquippedWeapon = Weapon;
}
```

```
void AWeapon::Equip(USceneComponent *InParent, FName InSocketName, AActor *NewOwner, APawn *NewInstigator) {
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	DisableSphereCollision();
	PlayEquipSound();
	DeactivateEmbers();
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	if (ActorIsSameType(OtherActor)) return;

	FHitResult BoxHit;
	BoxTrace(BoxHit);

	if (BoxHit.GetActor()) {
		if (ActorIsSameType(BoxHit.GetActor())) return;

		UGameplayStatics::ApplyDamage(BoxHit.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		ExecuteGetHit(BoxHit);
		CreateFields(BoxHit.ImpactPoint);
	}
}

void AWeapon::ExecuteGetHit(FHitResult &BoxHit) {
	IHitInterface *HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface) {
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}

void AWeapon::BoxTrace(FHitResult &BoxHit) {
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}
```

### 보물 (금화)
![](/README/treasure.gif)

```
void ATreasure::OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	IPickupInterface *PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface) {
		PickupInterface->AddGold(this);

		SpawnPickupSound();
		Destroy();
	}
}
```

### 소울
![](/README/soul.gif)

```
void ASoul::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	const double LocationZ = GetActorLocation().Z;
	if (LocationZ > DesiredZ) {
		const FVector DeltaLocation = FVector(0.f, 0.f, DriftRate * DeltaTime);
		AddActorWorldOffset(DeltaLocation);
	}
}

void ASoul::BeginPlay() {
	Super::BeginPlay();

	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0.f, 0.f, 2000.f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	TArray<AActor *> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	DesiredZ = HitResult.ImpactPoint.Z + 50.f;
}

void ASoul::OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	IPickupInterface *PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface) {
		PickupInterface->AddSouls(this);

		SpawnPickupSystem();
		SpawnPickupSound();

		Destroy();
	}
}
```

## 적
### 순찰
![](/README/patrol.gif)

```
void AEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling) {
		CheckCombatTarget();
	} else {
		CheckPatrolTarget();
	}
}

void AEnemy::BeginPlay() {
	Super::BeginPlay();

	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	InitializeEnemy();
	Tags.Add(FName("Enemy"));
}

void AEnemy::InitializeEnemy() {
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	HideHealthBar();
	SpawnDefaultWeapon();
}

void AEnemy::CheckPatrolTarget() {
	if (InTargetRange(PatrolTarget, PatrolRadius)) {
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}

AActor *AEnemy::ChoosePatrolTarget() {
	TArray<AActor *> ValidTargets;
	for (AActor *Target : PatrolTargets) {
		if (Target != PatrolTarget) {
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0) {
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}
```

### 플레이어 감지 및 추적
![](/README/chase.gif)

```
void AEnemy::CheckCombatTarget() {
	if (IsOutsideCombatRadius()) {
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged()) StartPatrolling();
	} else if (IsOutsideAttackRadius() && !IsChasing()) {
		ClearAttackTimer();
		if (!IsEngaged()) ChaseTarget();
	} else if (CanAttack()) {
		StartAttackTimer();
	}
}

void AEnemy::PawnSeen(APawn *SeenPawn) {
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("EngageableTarget"));

	if (bShouldChaseTarget) {
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}
```

### 모션 워핑을 이용한 공격
![](/README/motionwarpoing.gif)

```
FVector ABaseCharacter::GetTranslationWarpTarget() {
	if (CombatTarget == nullptr) return FVector();

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	return CombatTargetLocation + TargetToMe;
}

FVector ABaseCharacter::GetRotationWarpTarget() {
	if (CombatTarget) {
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}
```

## 화면
### 체력바
```
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
```

### 스테미너바
```
void ARPGCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (Attributes && RPGOverlay) {
        Attributes->RegenStamina(DeltaTime);
        RPGOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
    }
}
```

### 금화 표시
```
void ARPGCharacter::AddGold(ATreasure *Treasure) {
    if (Attributes && RPGOverlay) {
        Attributes->AddGold(Treasure->GetGold());
        RPGOverlay->SetGold(Attributes->GetGold());
    }
}

```

### 소울 표시
```
void ARPGCharacter::AddSouls(ASoul *Soul) {
    if (Attributes && RPGOverlay) {
        Attributes->AddSouls(Soul->GetSouls());
        RPGOverlay->SetSouls(Attributes->GetSouls());
    }
}
```

# 시행 착오
> 초기에는 플레이어 캐릭터를 언리얼 마켓플레이스에서 무료로 사용이 가능한 파라곤 캐릭터를 사용하고자 했습니다. 파라곤 캐릭터들은 무기가 같이 딸려오기 때문에 무기가 없는 상태를 구현하고자 방법을 찾았습니다. 첫번째 방법은 무기 재질을 투명하게 하는 방법, 두번째는 무기의 부모 본을 (0,0,0)으로 축소하는 방법이었습니다. 두번째 방법을 먼저 시도했는데 언리얼 엔진을 처음 다루는 시점이었기에 어떻게 블루프린트를 구성해야하는지조차 알기가 쉽지 않았습니다(애님그래프에서 Transform (Modify) Bone 노드를 만들고 Bone to Modify, Scale, Scale Mode를 설정하면 가능합니다.). 첫번째 방법은 무기 재질을 투명하게 만들었기 때문에 무기가 없는 상태에서의 애니메이션을 리타겟으로 추가해야했습니다. 유튜브에서 찾아보며 애니메이션 리타겟을 하는것까지 성공을 하였으나 착지 시에 땅까지 축소되었다가 다시 원상태로 복귀되는 문제가 발생하였고, 애니메이션 리타겟 과정에서 문제가 생긴것인가 싶어 기본 언리얼 스타트 팩에 있는 캐릭터를 사용하여 프로젝트를 시작했습니다.

> 언리얼 기본 캐릭터를 사용하여 점프-점프루프-착지 애니메이션을 구성하는 중에 착지 시에 캐릭터의 몸체가 땅까지 축소되었다가 다시 원상태를 되찾는 문제가 생겼습니다. 애니메이션 리타겟의 문제인가 싶어 리타겟을 여러번 재시도 해보기도 하였고, 메쉬를 바꿔서 시도하기도 했으나 실패했습니다. 그러다 같은 문제를 겪다가 해결한 분의 블로그 글을 보게 되었고 문제는 착지 애니메이션의 설정을 No Addtive로 하여 해결하였습니다. Additive Animation은 기본 애니메이션과의 차이를 현재 재생 중인 애니메이션에 더해주는 방식으로 동작하는데, 착지 애니메이션을 Additive Animation으로 설정했지만 Apply Additive 노드를 사용하지 않고 그대로 재생했기 때문에 문제가 발생한 것이었습니다. Apply Additive 노드를 사용하면 기본 애니메이션과 Additive Animation의 차이를 계산하여 자연스럽게 블랜딩해주는데 이 과정을 거치지 않아 캐릭터가 부자연스럽게 땅으로 축소했다가 복귀하는 문제가 발생했고, 따라서 Additive Animation을 사용할 때는 반드시 Apply Additive 노드를 통해 기본 애니메이션에 적용해주는 과정이 필요하며, 그렇지 않다면 애초에 Additive를 사용하지 않도록 설정해야 함을 알게 되었습니다.
