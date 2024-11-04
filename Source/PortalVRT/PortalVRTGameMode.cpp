// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortalVRTGameMode.h"
#include "PortalVRTCharacter.h"
#include "UObject/ConstructorHelpers.h"

APortalVRTGameMode::APortalVRTGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void APortalVRTGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    APortalVRTCharacter* PlayerCharacter = Cast<APortalVRTCharacter>(NewPlayer->GetPawn());
    if (PlayerCharacter)
    {
        FString PlayerName = FString::Printf(TEXT("Player%d"), PlayerCounter);
        PlayerCharacter->PlayerName = PlayerName;

        PlayerCounter++;

        if (PlayerCounter > 100) {
            PlayerCounter = 0;
        }
    }

}