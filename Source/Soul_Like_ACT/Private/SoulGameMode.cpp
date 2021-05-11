// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SoulGameMode.h"
#include "SoulGameState.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/SoulPlayerCharacter.h"
#include "Player/SoulPlayerController.h"

ASoulGameMode::ASoulGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<ASoulPlayerCharacter> PlayerPawnBPClass(
        TEXT("/Game/Blueprints/Player/Player_Character_BP"));
    static ConstructorHelpers::FClassFinder<ASoulPlayerController> PlayerControllerBPClass(
        TEXT("/Game/Blueprints/Player/SoulPlayerController_BP"));
    static ConstructorHelpers::FClassFinder<ASoulGameState> GameStateBPClass(TEXT("/Game/Blueprints/SoulGameState"));

    if (PlayerPawnBPClass.Class != NULL)
        DefaultPawnClass = PlayerPawnBPClass.Class;

    if (PlayerControllerBPClass.Class != NULL)
        PlayerControllerClass = PlayerControllerBPClass.Class;

    if (GameStateBPClass.Class != NULL)
        GameStateClass = GameStateBPClass.Class;
}
