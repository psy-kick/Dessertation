// Copyright Epic Games, Inc. All Rights Reserved.


#include "TBAiGameModeBase.h"
#include "PartyBase.h"
#include "EnemyBase.h"
#include "SelectionPointer.h"
#include "ActionUI.h"
#include "Blueprint/UserWidget.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include <Kismet/GameplayStatics.h>

//constructor
ATBAiGameModeBase::ATBAiGameModeBase()
{
	CurrentState = ETurnState::StartTurn;
    UClass* SelectionPointerClass = LoadClass<USelectionPointer>(nullptr, TEXT("/Game/Blueprints/UI/Pointer_BP.Pointer_BP_C"));
    PointerHUDClass = SelectionPointerClass;
    UClass* ActionClass = LoadClass<UActionUI>(nullptr, TEXT("/Game/Blueprints/UI/BP_ActionUI.BP_ActionUI_C"));
    ActionUIClass = ActionClass;
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
    case ETurnState::WaitTurn:
        WaitTurn();
        break;
    case ETurnState::PlayerAttack:
        PlayerAttack();
        break;
    case ETurnState::EnemyTurn:
        EnemyTurn();
        break;
    case ETurnState::Won:
        Won();
        break;
    case ETurnState::Lost:
        Lost();
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
    if (GetTotalEnemyHp() > 0)
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
                    if (PlayerController && isCharSelectable == true)
                    {
                        PlayerController->InputComponent->BindAction("MoveUp", IE_Pressed, this, &ATBAiGameModeBase::MoveSelectedUp);
                        PlayerController->InputComponent->BindAction("MoveDown", IE_Pressed, this, &ATBAiGameModeBase::MoveSelectedDown);
                    }
                }
                else if (SelectedParty == NULL)
                {
                    if (ActionUI->IsInViewport())
                    {
                        ActionUI->RemoveFromViewport();
                    }
                }
            }
            else
            {
                CurrentState = ETurnState::Lost;
                HandleStates(CurrentState);
            }
        }
        CurrentState = ETurnState::WaitTurn;
        HandleStates(CurrentState);
    }
    else
    {
        CurrentState = ETurnState::Won;
        HandleStates(CurrentState);
    }
}

#pragma region PlayerSelections
void ATBAiGameModeBase::MoveSelectedUp()
{
    isCharSelectable = true;
    SelectionIndex = (SelectionIndex - 1 + FoundPartyActors.Num()) % FoundPartyActors.Num();
    UpdateSelection();
}
void ATBAiGameModeBase::MoveSelectedDown()
{
    isCharSelectable = true;
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
        if (SelectedParty->HP > 0)
        {
            SelectedParty->AttackEnemy();
            CurrentState = ETurnState::EnemyTurn;
            HandleStates(CurrentState);
        }
        else
        {
            SelectedParty->Destroy();
        }
    }
    else
    {
        // Handle the case when SelectedPartyInstance is not valid.
        UE_LOG(LogTemp, Error, TEXT("SelectedPartyInstance is not valid."));
    }
}

int ATBAiGameModeBase::GetTotalPartyHp()
{
    UWorld* World = GetWorld();
    if (World)
    {
        UGameplayStatics::GetAllActorsOfClass(World, APartyBase::StaticClass(), FoundPartyActors);
        for (AActor* party : FoundPartyActors)
        {
            APartyBase* PartyInstance = Cast<APartyBase>(party);
            if (PartyInstance)
            {
                TotalPartyHp += PartyInstance->CalculateTotalPartyHP();
            }
        }
    }
    return TotalPartyHp;
}

int ATBAiGameModeBase::GetTotalEnemyHp()
{
    UWorld* World = GetWorld();
    if (World)
    {
        UGameplayStatics::GetAllActorsOfClass(World, AEnemyBase::StaticClass(), FoundEnemyActors);
        for (AActor* party : FoundEnemyActors)
        {
            AEnemyBase* EnemyInstance = Cast<AEnemyBase>(party);
            if (EnemyInstance)
            {
                TotalEnemyHp += EnemyInstance->CalculateTotalEnemyHP();
            }
        }
    }
    return TotalEnemyHp;
}

void ATBAiGameModeBase::EnemyTurn()
{
    UE_LOG(LogTemp, Warning, TEXT("EnemyTurn"));
    if (ActionUI->IsInViewport())
    {
        ActionUI->RemoveFromParent();
    }
    if (GetTotalEnemyHp() > 0)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            UGameplayStatics::GetAllActorsOfClass(World, AEnemyBase::StaticClass(), FoundEnemyActors);
            if (FoundEnemyActors.Num() > 0)
            {
                int32 RandomIndex = FMath::RandRange(0, FoundEnemyActors.Num() - 1);
                AEnemyBase* RandomEnemy = Cast<AEnemyBase>(FoundEnemyActors[RandomIndex]);
                if (RandomEnemy)
                {
                    if (RandomEnemy->HP > 0)
                    {
                        RandomEnemy->EnemyAttack();
                    }
                    else
                    {
                        RandomEnemy->Destroy();
                    }
                }
            }
            else
            {
                CurrentState = ETurnState::Won;
                HandleStates(CurrentState);
            }
        }
        if (GetTotalPartyHp() > 0)
        {
            CurrentState = ETurnState::PlayerTurn;
            HandleStates(CurrentState);
        }
        else
        {
            CurrentState = ETurnState::Lost;
            HandleStates(CurrentState);
        }
    }
    else
    {

        CurrentState = ETurnState::Lost;
        HandleStates(CurrentState);
    }
}

void ATBAiGameModeBase::WaitTurn()
{
    APartyBase* SelectedParty = UpdateSelection();
    if (SelectedParty)
    {
        if (ActionUIClass)
        {
            ActionUI = CreateWidget<UActionUI>(GetWorld(), ActionUIClass);
            ActionUI->AddToViewport();
        }
    }
}

void ATBAiGameModeBase::Won()
{
    UE_LOG(LogTemp, Error, TEXT("Won"));
}

void ATBAiGameModeBase::Lost()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Lost"));
    UE_LOG(LogTemp, Error, TEXT("Lost"));
}

void ATBAiGameModeBase::EndTurn()
{
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("EndTurn"));
}