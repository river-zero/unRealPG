#include "Enemies/FEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "For_The_Job/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/FAttributeComponent.h"
#include "UI/FHealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

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

	Attributes = CreateDefaultSubobject<UFAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UFHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	// 캐릭터의 이동과 회전이 자연스럽도록 함
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AFEnemy::BeginPlay() {
	Super::BeginPlay();

	// 시작 시에 체력바 보이지 않게 설정
	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(false);
	}

	EnemyController = Cast<AAIController>(GetController());
	if (EnemyController && PatrolTarget) {
		// 목표 지점 및 허용 반경
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(PatrolTarget);
		MoveRequest.SetAcceptanceRadius(15.f);

		// 이동 요청
		FNavPathSharedPtr NavPath;
		EnemyController->MoveTo(MoveRequest, &NavPath);

		// 경로의 모든 지점 시각적 표시
		TArray<FNavPathPoint> &PathPoints = NavPath->GetPathPoints();
		for (auto &Point : PathPoints) {
			const FVector &Location = Point.Location;
			DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
		}
	}
}

void AFEnemy::Die() {
	UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DieMontage) {
		AnimInstance->Montage_Play(DieMontage);

		const int32 Selection = FMath::RandRange(0, 4);
		FName SelectionName = FName();
		switch (Selection) {
		case 0:
			SelectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 1:
			SelectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 2:
			SelectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Death3;
			break;
		case 3:
			SelectionName = FName("Death4");
			DeathPose = EDeathPose::EDP_Death4;
			break;
		case 4:
			SelectionName = FName("Death5");
			DeathPose = EDeathPose::EDP_Death5;
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SelectionName, DieMontage);
	}

	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(false);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.f);
}

void AFEnemy::PlayHitReactMontage(const FName &SectionName) {
	UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage) {
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

bool AFEnemy::InTargetRange(AActor *Target, double Radius) {
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_SPHERE_SingleFrame(Target->GetActorLocation());

	return DistanceToTarget <= Radius;
}

void AFEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// 적에서 공격을 준 대상까지의 거리 계산
	if (CombatTarget) {
		// 일정거리 이상 벌어지면 체력바 숨김
		if (!InTargetRange(CombatTarget, CombatRadius)) {
			CombatTarget = nullptr;
			if (HealthBarWidget) {
				HealthBarWidget->SetVisibility(false);
			}
		}
	}

	// 랜덤으로 정찰 지점 이동
	if (PatrolTarget && EnemyController) {
		// 정찰 지점에 도착했다면 랜덤으로 새 정찰 지점을 선택해 이동
		if (InTargetRange(PatrolTarget, PatrolRadius)) {
			// 정찰 지점 중복 방지
			TArray<AActor*> ValidTargets;
			for (AActor *Target : PatrolTargets) {
				if (Target != PatrolTarget) {
					ValidTargets.AddUnique(Target);
				}
			}

			const int32 NumPatrolTargets = PatrolTargets.Num();
			if (NumPatrolTargets > 0) {
				const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
				AActor *Target = PatrolTargets[TargetSelection];
				PatrolTarget = Target;

				FAIMoveRequest MoveRequest;
				MoveRequest.SetGoalActor(PatrolTarget);
				MoveRequest.SetAcceptanceRadius(15.f);
				EnemyController->MoveTo(MoveRequest);
			}
		}
	}
}

void AFEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFEnemy::GetHit_Implementation(const FVector &ImpactPoint) {
	// DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);

	// 공격받으면 체력바 표시
	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(true);
	}

	// 체력이 있다면 공격받고, 없다면 죽음
	if (Attributes && Attributes->IsAlive()) {
		DirectionalHitReact(ImpactPoint);
	} else {
		Die();
	}

	// 피격음 재생
	if (HitSound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}

	// 혈액 효과
	if (HitParticles) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

void AFEnemy::DirectionalHitReact(const FVector &ImpactPoint) {
	// 내적을 이용해 전방 벡터와 타격 지점까지의 각도 계산
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	// 외적을 이용해 각도의 양수/음수 판별
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0) {
		Theta *= -1.f;
	}

	FName Section("FromBack");
	if (Theta >= -45.f && Theta < 45.f) {
		Section = FName("FromFront");
	} else if (Theta >= -135.f && Theta < -45.f) {
		Section = FName("FromLeft");
	} else if (Theta >= 45.f && Theta < 135.f) {
		Section = FName("FromRight");
	}
	PlayHitReactMontage(Section);

	/*
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
	*/
}

float AFEnemy::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser) {
	if (Attributes && HealthBarWidget) {
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}

	CombatTarget = EventInstigator->GetPawn();

	return DamageAmount;
}
