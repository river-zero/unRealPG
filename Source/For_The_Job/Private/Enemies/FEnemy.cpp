#include "Enemies/FEnemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/FAttributeComponent.h"
#include "UI/FHealthBarComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Items/Weapons/FWeapon.h"
// #include "For_The_Job/DebugMacros.h"
// #include "Kismet/KismetSystemLibrary.h"

AFEnemy::AFEnemy() {
	PrimaryActorTick.bCanEverTick = true;

	float CharacterHalfHeight = 87.f;
	float CharacterRadius = 21.f;
	GetCapsuleComponent()->InitCapsuleSize(CharacterRadius, CharacterHalfHeight);
	FVector PivotPosition(0.f, 0.f, -CharacterHalfHeight);
	FRotator PivotRotation(0.f, -90.f, 0.f);
	GetMesh()->SetRelativeLocationAndRotation(PivotPosition, PivotRotation);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBarWidget = CreateDefaultSubobject<UFHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	// 캐릭터의 이동과 회전이 자연스럽도록 함
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

void AFEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling) {
		CheckCombatTarget();
	} else {
		CheckPatrolTarget();
	}
}

void AFEnemy::GetHit_Implementation(const FVector &ImpactPoint) {
	// DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);

	ShowHealthBar();

	// 체력이 있다면 공격받고, 없다면 죽음
	if (IsAlive()) {
		DirectionalHitReact(ImpactPoint);
	} else {
		Die();
	}

	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

float AFEnemy::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser) {
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	ChaseTarget();
	return DamageAmount;
}

void AFEnemy::Destroyed() {
	if (EquippedWeapon) {
		// 적 죽음 시 무기도 파괴
		EquippedWeapon->Destroy();
	}
}

void AFEnemy::BeginPlay() {
	Super::BeginPlay();

	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AFEnemy::PawnSeen);
	InitializeEnemy();
}

void AFEnemy::SpawnDefaultWeapon() {
	UWorld *World = GetWorld();
	if (World) {
		AFWeapon *DefaultWeapon = World->SpawnActor<AFWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

void AFEnemy::Die() {
	EnemyState = EEnemyState::EES_Dead;
	PlayDeathMontage();
	ClearAttackTimer();
	HideHealthBar();
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AFEnemy::Attack() {
	EnemyState = EEnemyState::EES_Engaged;
	Super::Attack();
	PlayAttackAnimation();
}

bool AFEnemy::CanAttack() {
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();

	return bCanAttack;
}

void AFEnemy::HandleDamage(float DamageAmount) {
	Super::HandleDamage(DamageAmount);

	if (Attributes && HealthBarWidget) {
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

int32 AFEnemy::PlayDeathMontage() {
	const int32 Selection = Super::PlayDeathMontage();
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX) {
		DeathPose = Pose;
	}

	return Selection;
}

void AFEnemy::AttackEnd() {
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AFEnemy::InitializeEnemy() {
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	HideHealthBar();
	SpawnDefaultWeapon();
}

void AFEnemy::CheckPatrolTarget() {
	// 랜덤 타겟 지정하고 시간 지연 후 이동
	if (InTargetRange(PatrolTarget, PatrolRadius)) {
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AFEnemy::PatrolTimerFinished, WaitTime);
	}
}

void AFEnemy::CheckCombatTarget() {
	if (CombatTarget) {
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
}

void AFEnemy::PatrolTimerFinished() {
	MoveToTarget(PatrolTarget);
}

void AFEnemy::HideHealthBar() {
	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(false);
	}
}

void AFEnemy::ShowHealthBar() {
	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(true);
	}
}

void AFEnemy::LoseInterest() {
	CombatTarget = nullptr;
	HideHealthBar();
}

void AFEnemy::StartPatrolling() {
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}

void AFEnemy::ChaseTarget() {
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool AFEnemy::IsOutsideCombatRadius() {
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AFEnemy::IsOutsideAttackRadius() {
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AFEnemy::IsInsideAttackRadius() {
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AFEnemy::IsChasing() {
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AFEnemy::IsAttacking() {
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AFEnemy::IsDead() {
	return EnemyState == EEnemyState::EES_Dead;
}

bool AFEnemy::IsEngaged() {
	return EnemyState == EEnemyState::EES_Engaged;
}

void AFEnemy::ClearAttackTimer() {
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AFEnemy::StartAttackTimer() {
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AFEnemy::Attack, AttackTime);
}

void AFEnemy::ClearPatrolTimer() {
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

bool AFEnemy::InTargetRange(AActor *Target, double Radius) {
	if (Target == nullptr) return false;

	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	// DRAW_SPHERE_SingleFrame(GetActorLocation());
	// DRAW_SPHERE_SingleFrame(Target->GetActorLocation());

	return DistanceToTarget <= Radius;
}

void AFEnemy::MoveToTarget(AActor *Target) {
	if (EnemyController == nullptr || Target == nullptr) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(40.f);
	EnemyController->MoveTo(MoveRequest);
}

AActor *AFEnemy::ChoosePatrolTarget() {
	TArray<AActor*> ValidTargets;
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

void AFEnemy::PawnSeen(APawn *SeenPawn) {
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
