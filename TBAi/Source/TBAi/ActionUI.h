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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* NameText;
	class AUnitBase* UnitName;
	UFUNCTION(BlueprintCallable)
	UTextBlock* SetName(FText UnitsName);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* AttackButton;
	UFUNCTION()
	void OnButtonClicked();
};
