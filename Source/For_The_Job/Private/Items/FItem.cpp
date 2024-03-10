#include "Items/FItem.h"
#include "For_The_Job/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/FRPGCharacter.h"

AFItem::AFItem() {
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
}

void AFItem::BeginPlay() {
	Super::BeginPlay();

	// 델리게이트에 함수를 바인딩해 이벤트 핸들링 가능
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AFItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AFItem::OnSphereEndOverlap);
}

float AFItem::TransformedSin() {
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AFItem::TransformedCos() {
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AFItem::OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	// 겹친 액터가 AFRPGCharacter 클래스의 인스턴스인 경우 해당 캐릭터의 OverlappingItem 멤버 변수를 현재 아이템으로 설정
	AFRPGCharacter *RPGCharacter = Cast<AFRPGCharacter>(OtherActor);
	if (RPGCharacter) {
		RPGCharacter->SetOverlappingItem(this);
	}
}

void AFItem::OnSphereEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) {
	AFRPGCharacter *RPGCharacter = Cast< AFRPGCharacter>(OtherActor);
	if (RPGCharacter) {
		RPGCharacter->SetOverlappingItem(nullptr);
	}
}

void AFItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	// 아이템이 호버링하는 동안 위아래로 움직이도록 함
	if (ItemState == EItemState::EIS_Hovering) {
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}
