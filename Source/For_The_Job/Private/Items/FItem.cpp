#include "Items/FItem.h"
#include "For_The_Job/DebugMacros.h"
#include "Components/SphereComponent.h"

AFItem::AFItem() {
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
}

void AFItem::BeginPlay() {
	Super::BeginPlay();

	// ��������Ʈ�� �Լ��� ���ε��� �̺�Ʈ �ڵ鸵 ����
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
	// UPrimitiveComponent�� �ٸ� ������Ʈ�� ������ �� ȣ��Ǵ� �ݹ� �Լ�

	const FString OtherActorName = OtherActor->GetName();
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
	}
}

void AFItem::OnSphereEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) {
	const FString OtherActorName = OtherActor->GetName();
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Blue, OtherActorName);
	}
}

void AFItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
}
