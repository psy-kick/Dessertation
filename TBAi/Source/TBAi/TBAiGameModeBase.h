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
	PlayerAttack,
	WaitTurn,
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
	int32 SelectionIndex;
	TArray<AActor*> FoundPartyActors;
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USelectionPointer> PointerHUDClass;
	UPROPERTY()
	class USelectionPointer* PointerHUD;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UActionUI> ActionUIClass;
	UPROPERTY()
	class UActionUI* ActionUI;
	APartyBase* SelectedPartyInstance;
//private functions
private:
	UFUNCTION()
	void HandleStates(ETurnState NewState);
	UFUNCTION()
	void StartTurn();
	UFUNCTION()
	void PlayerTurn();
	UFUNCTION()
	void PlayerAttack();
	UFUNCTION()
	void MoveSelectedUp();
	UFUNCTION()
	void MoveSelectedDown();
	UFUNCTION()
	APartyBase* UpdateSelection();
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
