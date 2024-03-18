#include "Characters/FCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/FWeapon.h"
#include "Components/FAttributeComponent.h"

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

void AFCharacter::Attack() {
}

void AFCharacter::Die() {
}

void AFCharacter::PlayAttackAnimation() {
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

void AFCharacter::AttackEnd() {
}

void AFCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
