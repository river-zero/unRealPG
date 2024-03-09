#include "Enemies/FEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "For_The_Job/DebugMacros.h"

AFEnemy::AFEnemy() {
	PrimaryActorTick.bCanEverTick = true;

	float CharacterHalfHeight = 86.f;
	float CharacterRadius = 20.f;

	GetCapsuleComponent()->InitCapsuleSize(CharacterRadius, CharacterHalfHeight);

	FVector PivotPosition(0.f, 0.f, -CharacterHalfHeight);
	FRotator PivotRotation(0.f, -90.f, 0.f);
	GetMesh()->SetRelativeLocationAndRotation(PivotPosition, PivotRotation);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void AFEnemy::BeginPlay() {
	Super::BeginPlay();
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
}

void AFEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFEnemy::GetHit(const FVector &ImpactPoint) {
	DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);
	PlayHitReactMontage(FName("FromLeft"));
}
