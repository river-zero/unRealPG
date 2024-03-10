#include "Items/FTreasure.h"
#include "Characters/FRPGCharacter.h"
#include "Kismet/GameplayStatics.h"

void AFTreasure::OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	// 원치 않는 기능이므로 Super::OnSphereOverlap(); 부르지 않음

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
