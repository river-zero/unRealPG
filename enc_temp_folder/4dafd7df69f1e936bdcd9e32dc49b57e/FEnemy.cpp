#include "Enemies/FEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "For_The_Job/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/FAttributeComponent.h"
#include "UI/FHealthBarComponent.h"

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
}

void AFEnemy::BeginPlay() {
	Super::BeginPlay();

	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(false);
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

void AFEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// ������ ������ �� �������� �Ÿ� ���
	if (CombatTarget) {
		const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();

		// �����Ÿ� �̻� �������� ü�¹� ����
		if (DistanceToTarget > CombatRadius) {
			CombatTarget = nullptr;
			if (HealthBarWidget) {
				HealthBarWidget->SetVisibility(false);
			}
		}
	}
}

void AFEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFEnemy::GetHit_Implementation(const FVector &ImpactPoint) {
	// DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);

	// ���ݹ����� ü�¹� ǥ��
	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(true);
	}

	// ü���� �ִٸ� ���ݹް�, ���ٸ� ����
	if (Attributes && Attributes->IsAlive()) {
		DirectionalHitReact(ImpactPoint);
	} else {
		Die();
	}

	// �ǰ��� ���
	if (HitSound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}

	// ���� ȿ��
	if (HitParticles) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

void AFEnemy::DirectionalHitReact(const FVector &ImpactPoint) {
	// ������ �̿��� ���� ���Ϳ� Ÿ�� ���������� ���� ���
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	// ������ �̿��� ������ ���/���� �Ǻ�
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
