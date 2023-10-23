// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class TBAI_API AEnemyBase : public AUnitBase
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AEnemyBase();
	int32 TotalEnemyMp = 0;
private:
	UPROPERTY(EditAnywhere, Category = "Stats")
	int MP = 0;
	TArray<AActor*> FoundActors;
public:
	UFUNCTION()
	int CalculateTotalEnemyMP();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
