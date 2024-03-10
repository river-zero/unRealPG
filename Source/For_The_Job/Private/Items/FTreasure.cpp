#include "Items/FTreasure.h"
#include "Characters/FRPGCharacter.h"
#include "Kismet/GameplayStatics.h"

void AFTreasure::OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	// ��ġ �ʴ� ����̹Ƿ� Super::OnSphereOverlap(); �θ��� ����

	AFRPGCharacter *RPGCharacter = Cast<AFRPGCharacter>(OtherActor);
	if (RPGCharacter) {
		if (PickupSound) {
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickupSound,
				GetActorLocation()
			);
		}

		Destroy();
	}
}
