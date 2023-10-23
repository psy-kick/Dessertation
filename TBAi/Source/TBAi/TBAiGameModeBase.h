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
	PlayerTurn,
	EnemyTurn,
	Won,
	Lost
};
UCLASS()
class TBAI_API ATBAiGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	ATBAiGameModeBase();
//private variable
private:
	UPROPERTY(EditAnywhere, Category = "Tracker")
	ETurnState CurrentState;
	UPROPERTY(VisibleAnywhere, Category = "Units")
	class APartyBase* PartyBase;
	class AEnemyBase* EnemyBase;
//private functions
private:
	UFUNCTION()
	void StartTurn();
	UFUNCTION()
	void PlayerTurn();
	UFUNCTION()
	void EnemyTurn();
	UFUNCTION()
	void WaitTurn();
	UFUNCTION()
	void EndTurn();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
