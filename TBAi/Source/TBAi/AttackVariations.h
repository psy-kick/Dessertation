// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionUI.h"
#include "AttackVariations.generated.h"

/**
 * 
 */
UCLASS()
class TBAI_API UAttackVariations : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* HeavyAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* LightAttack;
	class APartyBase* PartyBaseInstance;
	class ATBAiGameModeBase* GameModeInstance;
private:
	UFUNCTION()
	void OnHeavyButtonClicked();
	UFUNCTION()
	void OnLightButtonClicked();
};
