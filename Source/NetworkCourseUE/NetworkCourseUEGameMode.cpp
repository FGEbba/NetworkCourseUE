// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetworkCourseUEGameMode.h"
#include "NetworkCourseUEHUD.h"
#include "NetworkCourseUECharacter.h"
#include "UObject/ConstructorHelpers.h"

ANetworkCourseUEGameMode::ANetworkCourseUEGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ANetworkCourseUEHUD::StaticClass();
}
