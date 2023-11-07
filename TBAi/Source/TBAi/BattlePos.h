// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitBase.h"
#include "BattlePos.generated.h"

UCLASS()
class TBAI_API ABattlePos : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattlePos();
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AUnitBase> UnitRef;
	UPROPERTY(EditAnywhere, Category = "UnitPlacement")
	bool isEnemyPlacement;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
