#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FHealthBar.generated.h"

UCLASS()
class FOR_THE_JOB_API UFHealthBar : public UUserWidget {
	GENERATED_BODY()

public:
	// 블루프린트와 C++ 바인딩을 위하므로 이름 일치 확인
	UPROPERTY(Meta = (BindWidget))
	class UProgressBar *HealthBar;
};
