// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionUI.h"
#include "UnitBase.h"
#include "Components/TextBlock.h"

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