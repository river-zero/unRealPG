#include "Animations/FAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/FRPGCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UFAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

	// 입력->게임->애니메이션 로직이므로 일단 시도하는 방식
	RPGCharacter = Cast<AFRPGCharacter>(TryGetPawnOwner());
	if (RPGCharacter) {
		RPGCharacterMovement = RPGCharacter->GetCharacterMovement();
	}
}

void UFAnimInstance::NativeUpdateAnimation(float DeltaTime) {
	Super::NativeUpdateAnimation(DeltaTime);

	if (RPGCharacterMovement) {
		GroundSpeed = UKismetMathLibrary::VSizeXY(RPGCharacterMovement->Velocity);
		IsFalling = RPGCharacterMovement->IsFalling();
		CharacterState = RPGCharacter->GetCharacterState();
	}
}

void UFAnimInstance::PlayAttackMontage() {
	if (true == ::IsValid(AttackMontage)) {
		if (false == Montage_IsPlaying(AttackMontage)) {
			Montage_Play(AttackMontage);
		}
	}
}

void UFAnimInstance::AnimNotify_CheckHit() {
	if (true == OnCheckHitDelegate.IsBound()) {
		OnCheckHitDelegate.Broadcast();
	}
}

void UFAnimInstance::AnimNotify_AttackEnd() {
	if (true == OnAttackEndDelegate.IsBound()) {
		OnAttackEndDelegate.Broadcast();
	}
}
