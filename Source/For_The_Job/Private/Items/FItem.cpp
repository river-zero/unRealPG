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
	// ��ģ ���Ͱ� AFRPGCharacter Ŭ������ �ν��Ͻ��� ��� �ش� ĳ������ OverlappingItem ��� ������ ���� ���������� ����
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

	// �������� ȣ�����ϴ� ���� ���Ʒ��� �����̵��� ��
	if (ItemState == EItemState::EIS_Hovering) {
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}
