#pragma once

#include "CoreMinimal.h"

// 디버깅을 위한 시각적 표시 매크로
#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, true);
