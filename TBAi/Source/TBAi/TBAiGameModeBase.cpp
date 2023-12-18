// Copyright Epic Games, Inc. All Rights Reserved.


#include "TBAiGameModeBase.h"
#include "PartyBase.h"
#include "EnemyBase.h"
#include "SelectionPointer.h"
#include "Blueprint/UserWidget.h"
#include <Kismet/GameplayStatics.h>

//constructor
ATBAiGameModeBase::ATBAiGameModeBase()
{
	CurrentState = ETurnState::StartTurn;
    UClass* SelectionPointerClass = LoadClass<USelectionPointer>(nullptr, TEXT("/Game/Blueprints/UI/Pointer_BP.Pointer_BP_C"));
    PointerHUDClass = SelectionPointerClass;
}
void ATBAiGameModeBase::BeginPlay()
{
	Super::BeginPlay();
    HandleStates(CurrentState);
}
void ATBAiGameModeBase::HandleStates(ETurnState NewState)
{
    switch (NewState)
    {
    case ETurnState::StartTurn:
        StartTurn();
        break;
    case ETurnState::PlayerTurn:
        PlayerTurn();
        break;
    case ETurnState::PlayerAttack:
        PlayerAttack();
        break;
    default:
        break;
    }
}
void ATBAiGameModeBase::StartTurn()
{
	if (CurrentState == ETurnState::StartTurn)
	{
        UWorld* World = GetWorld();
        if (World)
        {
            UGameplayStatics::GetAllActorsOfClass(World, APartyBase::StaticClass(), FoundPartyActors);

            TArray<AActor*> FoundEnemyActors;
            UGameplayStatics::GetAllActorsOfClass(World, AEnemyBase::StaticClass(), FoundEnemyActors);

            int32 TotalPartyMP = 0;
            int32 TotalEnemyMP = 0;

            // Calculate the total MP for PartyBase instances.
            for (AActor* PartyActor : FoundPartyActors)
            {
                APartyBase* PartyInstance = Cast<APartyBase>(PartyActor);
                if (PartyInstance)
                {
                    TotalPartyMP += PartyInstance->CalculateTotalPartyMP();
                }
            }

            // Calculate the total MP for EnemyBase instances.
            for (AActor* EnemyActor : FoundEnemyActors)
            {
                AEnemyBase* EnemyInstance = Cast<AEnemyBase>(EnemyActor);
                if (EnemyInstance)
                {
                    TotalEnemyMP += EnemyInstance->CalculateTotalEnemyMP();
                }
            }

            if (TotalPartyMP > TotalEnemyMP)
            {
                CurrentState = ETurnState::PlayerTurn;
                HandleStates(CurrentState);
            }
            else
            {
                CurrentState = ETurnState::EnemyTurn;
                HandleStates(CurrentState);
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("World is not valid."));
        }
	}
}

void ATBAiGameModeBase::PlayerTurn()
{
    UWorld* World = GetWorld();
    if (World)
    {
        UGameplayStatics::GetAllActorsOfClass(World, APartyBase::StaticClass(), FoundPartyActors);
        if (FoundPartyActors.Num() > 0)
        {
            SelectionIndex = 0;
            APartyBase* SelectedParty = Cast<APartyBase>(FoundPartyActors[SelectionIndex]);
            if (SelectedParty)
            {
                if (PointerHUDClass)
                {
                    PointerHUD = CreateWidget<USelectionPointer>(World, PointerHUDClass);
                    SelectedParty->WidgetComponent->SetWidgetClass(PointerHUDClass);
                }
                APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
                if (PlayerController)
                {
                    PlayerController->InputComponent->BindAction("MoveUp", IE_Pressed, this, &ATBAiGameModeBase::MoveSelectedUp);
                    PlayerController->InputComponent->BindAction("MoveDown",IE_Pressed, this, &ATBAiGameModeBase::MoveSelectedDown);
                }
            }
        }
    }
    CurrentState = ETurnState::PlayerAttack;
    HandleStates(CurrentState);
}
#pragma region PlayerSelections
void ATBAiGameModeBase::MoveSelectedUp()
{
    SelectionIndex = (SelectionIndex - 1 + FoundPartyActors.Num()) % FoundPartyActors.Num();
    UpdateSelection();
}
void ATBAiGameModeBase::MoveSelectedDown()
{
    SelectionIndex = (SelectionIndex + 1) % FoundPartyActors.Num();
    UpdateSelection();
}
APartyBase* ATBAiGameModeBase::UpdateSelection()
{
    SelectedPartyInstance = Cast<APartyBase>(FoundPartyActors[SelectionIndex]);

    if (SelectedPartyInstance)
    {
        SelectedPartyInstance->WidgetComponent->SetWidgetClass(PointerHUDClass);
    }
    return SelectedPartyInstance;
}
#pragma endregion

void ATBAiGameModeBase::PlayerAttack()
{
    APartyBase* SelectedParty = UpdateSelection();
    if (SelectedParty)
    {
        SelectedPartyInstance->AttackEnemy();
    }
    else
    {
        // Handle the case when SelectedPartyInstance is not valid.
        GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Yellow, TEXT("SelectedPartyInstance is not valid."));
    }
}
void ATBAiGameModeBase::EnemyTurn()
{
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("EnemyTurn"));
}

void ATBAiGameModeBase::WaitTurn()
{
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("WaitTurn"));
}

void ATBAiGameModeBase::EndTurn()
{
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("EndTurn"));
}
