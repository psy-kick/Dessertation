// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include <Components/WidgetComponent.h>
#include "Http.h"
#include "PartyBase.generated.h"

UCLASS()
class TBAI_API APartyBase : public AUnitBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APartyBase();
	int32 TotalPartyMp = 0;
	int32 TotalPartyHp = 0;
private:
	UPROPERTY(EditAnywhere, Category = "Stats")
	int MP = 0;
	TArray<AActor*> FoundActors;
	TArray<AActor*> FoundEnemies;
	UPROPERTY(EditAnywhere)
	TArray<APartyBase*> PartyList;
public:
	UPROPERTY(EditAnywhere, Category = "Widgets")
	UWidgetComponent* WidgetComponent;
	UPROPERTY(EditAnywhere)
	bool HeavyAttackFlag;
	UPROPERTY(EditAnywhere)
	bool LightAttackFlag;
	UPROPERTY(EditAnywhere)
	float RemainingHP;
public:
	UFUNCTION()
	int CalculateTotalPartyMP();
	UFUNCTION()
	float CalculateTotalPartyHP();
	UFUNCTION()
	void AttackEnemy();
	UFUNCTION()
	void HealPlayer();
	void OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	UFUNCTION()
	void SendRemainingHp();
	void GetRemainingHp();
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
