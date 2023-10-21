// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TBAiGameModeBase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETurnState : uint8
{
	StartTurn,
	WaitTurn,
	EndTurn
};
UCLASS()
class TBAI_API ATBAiGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	ATBAiGameModeBase();
private:
	UPROPERTY(EditAnywhere, Category="Tracker")
	ETurnState CurrentState;
	UFUNCTION()
	void StartTurn();
	UFUNCTION()
	void WaitTurn();
	UFUNCTION()
	void EndTurn();

};
