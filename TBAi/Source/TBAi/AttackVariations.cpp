// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackVariations.h"
#include "PartyBase.h"
#include "TBAiGameModeBase.h"
#include "Components/Button.h"

void UAttackVariations::NativeConstruct()
{
	if (HeavyAttack)
	{
		HeavyAttack->OnClicked.AddDynamic(this, &UAttackVariations::OnHeavyButtonClicked);
	}
	if (LightAttack)
	{
		LightAttack->OnClicked.AddDynamic(this, &UAttackVariations::OnLightButtonClicked);
	}
}
void UAttackVariations::OnHeavyButtonClicked()
{
	GameModeInstance = GetWorld()->GetAuthGameMode<ATBAiGameModeBase>();
	if (GameModeInstance)
	{
		GameModeInstance->isCharSelectable = false;
		GameModeInstance->SelectedPartyInstance->HeavyAttackFlag = true;
		GameModeInstance->PlayerAttack();
	}
}

void UAttackVariations::OnLightButtonClicked()
{
	GameModeInstance = GetWorld()->GetAuthGameMode<ATBAiGameModeBase>();
	if (GameModeInstance)
	{
		GameModeInstance->isCharSelectable = false;
		GameModeInstance->SelectedPartyInstance->LightAttackFlag = true;
		GameModeInstance->PlayerAttack();
	}
}
