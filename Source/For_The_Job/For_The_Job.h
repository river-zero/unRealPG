#pragma once

#include "CoreMinimal.h"

// ������� ���� �ð��� ǥ�� ��ũ��
#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, true);
