#include "Characters/FCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/FWeapon.h"
#include "Components/FAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
// #include "For_The_Job/DebugMacros.h"
// #include "Kismet/KismetSystemLibrary.h"

void AFCharacter::EnableBoxCollision() {
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox()) {
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AFCharacter::DisableBoxCollision() {
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox()) {
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

AFCharacter::AFCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UFAttributeComponent>(TEXT("Attributes"));
}

void AFCharacter::BeginPlay() {
	Super::BeginPlay();
}

void AFCharacter::GetHit_Implementation(const FVector &ImpactPoint, AActor *Hitter) {
	// DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);

	if (IsAlive() && Hitter) {
		DirectionalHitReact(Hitter->GetActorLocation());
	} else {
		Die();
	}

	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

void AFCharacter::Attack() {
}

void AFCharacter::Die() {
}

int32 AFCharacter::PlayAttackAnimation() {
	return PlayRandomMontageSection(AttackAnimation, AttackMontageSections);
}

int32 AFCharacter::PlayDeathMontage() {
	return PlayRandomMontageSection(DieMontage, DeathMontageSections);
}

void AFCharacter::StopAttackMontage() {
	UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		AnimInstance->Montage_Stop(0.25f, AttackAnimation);
	}
}

void AFCharacter::PlayHitReactMontage(const FName &SelectionName) {
	UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage) {
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SelectionName, HitReactMontage);
	}
}

void AFCharacter::DirectionalHitReact(const FVector &ImpactPoint) {
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

bool AFCharacter::CanAttack() {
	return false;
}

bool AFCharacter::IsAlive() {
	return Attributes && Attributes->IsAlive();
}

void AFCharacter::AttackEnd() {
}

void AFCharacter::PlayHitSound(const FVector &ImpactPoint) {
	if (HitSound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

void AFCharacter::SpawnHitParticles(const FVector &ImpactPoint) {
	if (HitParticles) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

void AFCharacter::HandleDamage(float DamageAmount) {
	if (Attributes) {
		Attributes->ReceiveDamage(DamageAmount);
	}
}

void AFCharacter::DisableCapsule() {
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFCharacter::PlayMontageSection(UAnimMontage *Montage, const FName &SectionName) {
	UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage) {
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 AFCharacter::PlayRandomMontageSection(UAnimMontage *Montage, const TArray<FName> &SectionNames) {
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

void AFCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
