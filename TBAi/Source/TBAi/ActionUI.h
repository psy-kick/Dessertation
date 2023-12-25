// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionUI.generated.h"

/**
 * 
 */
UCLASS()
class TBAI_API UActionUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (Binding))
	class UTextBlock* NameText;
	class AUnitBase* UnitName;
	void SetName(FText UnitsName);
};
