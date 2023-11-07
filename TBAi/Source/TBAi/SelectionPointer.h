// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionPointer.generated.h"

/**
 * 
 */
UCLASS()
class TBAI_API USelectionPointer : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetPointer();
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* PointerImage;
};
