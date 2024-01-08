// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "Http.h"
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
	int32 TotalEnemyHp = 0;
	UPROPERTY(EditAnywhere)
	bool HeavyAttackFlag;
	UPROPERTY(EditAnywhere)
	bool LightAttackFlag;
	UPROPERTY(EditAnywhere)
	bool Attacking;
	UPROPERTY(EditAnywhere)
	float RemainingPartyHP;
	APartyBase* SlectedParty;
	UPROPERTY(EditAnywhere, Category = "GptInstruction")
	FString DamageInstruction;
private:
	UPROPERTY(EditAnywhere, Category = "Stats")
	int MP = 0;
	TArray<AActor*> FoundActors;
	TArray<AActor*> FoundPartyActors;
public:
	UFUNCTION()
	int CalculateTotalEnemyMP();
	float CalculateTotalEnemyHP();
	UFUNCTION()
	void EnemyAttack();
	UFUNCTION()
	APartyBase* GetSelectedParty();
	UFUNCTION()
	void SendRemainingHp();
	void OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
private:
	UFUNCTION()
	void HeavyAttack();
	UFUNCTION()
	void LightAttack();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
