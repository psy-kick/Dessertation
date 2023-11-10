// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include <Components/WidgetComponent.h>
#include "PartyBase.generated.h"
UENUM(BlueprintType)
enum class EPlayerStates : uint8
{
	SelectHero,
	SelectEnemyOrAlly,
	Action,
	FinishTurn
};
UCLASS()
class TBAI_API APartyBase : public AUnitBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APartyBase();
	int32 TotalPartyMp = 0;
private:
	UPROPERTY(EditAnywhere, Category = "Stats")
	int MP = 0;
	TArray<AActor*> FoundActors;
	UPROPERTY(EditAnywhere, Category = "Tracker")
	EPlayerStates CurrentState;
	UPROPERTY(EditAnywhere)
	TArray<APartyBase*> PartyList;
public:
	UPROPERTY(EditAnywhere, Category = "Widgets")
	UWidgetComponent* WidgetComponent;
public:
	UFUNCTION()
	int CalculateTotalPartyMP();
	UFUNCTION()
	void SelectHero();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
