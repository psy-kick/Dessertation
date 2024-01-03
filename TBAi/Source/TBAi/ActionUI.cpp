// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionUI.h"
#include "UnitBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "TBAiGameModeBase.h"
#include "PartyBase.h"

void UActionUI::NativeConstruct()
{
	//if (AttackButton)
	//{
	//	AttackButton->OnClicked.AddDynamic(this, &UActionUI::OnButtonClicked);
	//}
}

UTextBlock* UActionUI::SetName(FText UnitsName)
{
	if (NameText)
	{
		if (UnitName)
		{
			UnitName->ActorName = UnitsName;
			NameText->SetText(UnitsName);
		}
	}
	return NameText;
}

void UActionUI::OnButtonClicked()
{
	//GameInstance = GetWorld()->GetAuthGameMode<ATBAiGameModeBase>();
	//if (GameInstance)
	//{
	//	GameInstance->PlayerAttack();
	//}
}
