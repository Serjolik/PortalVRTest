// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PortalVRTGameMode.generated.h"

UCLASS(minimalapi)
class APortalVRTGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APortalVRTGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	UPROPERTY()
	int32 PlayerCounter = 0;
};



