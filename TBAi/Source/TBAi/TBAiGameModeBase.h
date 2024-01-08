// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Http.h"
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
	UPROPERTY(VisibleAnywhere, Category = "Units")
	class APartyBase* PartyBase;
	class AEnemyBase* EnemyBase;
	int32 SelectionIndex;
	TArray<AActor*> FoundPartyActors;
	TArray<AActor*> FoundEnemyActors;
	float TotalPartyHp = 0;
	float TotalEnemyHp = 0;
	float MaxHp = 100;
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USelectionPointer> PointerHUDClass;
	UPROPERTY()
	class USelectionPointer* PointerHUD;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UActionUI> ActionUIClass;
	UPROPERTY()
	class UActionUI* ActionUI;
	TSubclassOf<class UVictoryWidget> VictoryUIClass;
	UPROPERTY()
	class UVictoryWidget* VictoryUI;
	TSubclassOf<class ULossWidget> LossUIClass;
	UPROPERTY()
	class ULossWidget* LossUI;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	APartyBase* SelectedPartyInstance;
	UPROPERTY(EditAnywhere, Category = "Tracker")
	ETurnState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isCharSelectable = true;
//private functions
private:
	UFUNCTION()
	void StartTurn();
	UFUNCTION()
	void PlayerTurn();
	UFUNCTION()
	void MoveSelectedUp();
	UFUNCTION()
	void MoveSelectedDown();
	UFUNCTION(BlueprintCallable)
	APartyBase* UpdateSelection();
	UFUNCTION()
	void WaitTurn();
	UFUNCTION()
	void EndTurn();
//public functions
public:
	UFUNCTION()
	void HandleStates(ETurnState NewState);
	UFUNCTION()
	void PlayerAttack();
	UFUNCTION()
	int GetTotalPartyHp();
	UFUNCTION()
	int GetTotalEnemyHp();
	UFUNCTION()
	void Lost();
	UFUNCTION()
	void Won();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
