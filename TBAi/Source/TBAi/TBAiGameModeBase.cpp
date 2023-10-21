// Copyright Epic Games, Inc. All Rights Reserved.


#include "TBAiGameModeBase.h"

//constructor
ATBAiGameModeBase::ATBAiGameModeBase()
{
	CurrentState = ETurnState::WaitTurn;
}

void ATBAiGameModeBase::StartTurn()
{
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("StartTurn"));
}

void ATBAiGameModeBase::WaitTurn()
{
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("WaitTurn"));
}

void ATBAiGameModeBase::EndTurn()
{
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("EndTurn"));
}
