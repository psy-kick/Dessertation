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
protected:
	virtual void NativeConstruct() override;
public:
	UFUNCTION()
	void OnButtonClicked();
	class ATBAiGameModeBase* GameInstance;
};
