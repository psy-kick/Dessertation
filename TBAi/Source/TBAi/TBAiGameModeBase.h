// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TBAiGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TBAI_API ATBAiGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UENUM(BlueprintType)
	enum class ETurnState : uint8
	{
		StartTurn,
		WaitTurn,
		EndTurn
	};
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
