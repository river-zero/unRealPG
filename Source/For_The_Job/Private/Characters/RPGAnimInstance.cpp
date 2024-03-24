#include "Characters/RPGAnimInstance.h"
#include "Characters/RPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void URPGAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

	RPGCharacter = Cast<ARPGCharacter>(TryGetPawnOwner());
	if (RPGCharacter) {
		RPGCharacterMovement = RPGCharacter->GetCharacterMovement();
	}
}

void URPGAnimInstance::NativeUpdateAnimation(float DeltaTime) {
	Super::NativeUpdateAnimation(DeltaTime);
	
	if (RPGCharacter) {
		GroundSpeed = UKismetMathLibrary::VSizeXY(RPGCharacterMovement->Velocity);
		IsFalling = RPGCharacterMovement->IsFalling();
		bJumpInput = RPGCharacter->GetJumpInput();
		CharacterState = RPGCharacter->GetCharacterState();
		ActionState = RPGCharacter->GetActionState();
		DeathPose = RPGCharacter->GetDeathPose();
	}
}
