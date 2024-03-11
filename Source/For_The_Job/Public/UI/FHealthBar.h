#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FHealthBar.generated.h"

UCLASS()
class FOR_THE_JOB_API UFHealthBar : public UUserWidget {
	GENERATED_BODY()

public:
	// �������Ʈ�� C++ ���ε��� ���ϹǷ� �̸� ��ġ Ȯ��
	UPROPERTY(Meta = (BindWidget))
	class UProgressBar *HealthBar;
};
