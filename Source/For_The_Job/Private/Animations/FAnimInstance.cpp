#include "Animations/FAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/FRPGCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UFAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

	// �Է�->����->�ִϸ��̼� �����̹Ƿ� �ϴ� �õ��ϴ� ���
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
	}
}
