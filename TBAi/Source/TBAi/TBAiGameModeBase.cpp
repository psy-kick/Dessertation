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
    PointerHUDClass = USelectionPointer::StaticClass();
}
void ATBAiGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	StartTurn();
}
void ATBAiGameModeBase::StartTurn()
{
	if (CurrentState == ETurnState::StartTurn)
	{
        UWorld* World = GetWorld();
        if (World)
        {
            TArray<AActor*> FoundPartyActors;
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
                PlayerTurn();
            }
            else
            {
                CurrentState = ETurnState::EnemyTurn;
                EnemyTurn();
            }
        }
        else
        {
            // Handle the case when the world is not valid.
            // You might want to log an error or take appropriate action here.
            UE_LOG(LogTemp, Error, TEXT("World is not valid."));
        }
	}
}

void ATBAiGameModeBase::PlayerTurn()
{
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("PlayerTurn"));
    UWorld* World = GetWorld();
    if (World)
    {
        TArray<AActor*> FoundPartyActors;
        UGameplayStatics::GetAllActorsOfClass(World, APartyBase::StaticClass(), FoundPartyActors);
        if (FoundPartyActors.Num() > 0)
        {
            // Generate a random index within the range of the array.
            int32 RandomIndex = FMath::RandRange(0, FoundPartyActors.Num() - 1);

            // Use the random index to access a random PartyBase instance.
            APartyBase* RandomPartyInstance = Cast<APartyBase>(FoundPartyActors[RandomIndex]);

            if (RandomPartyInstance)
            {
                if (PointerHUDClass)
                {
                    PointerHUD = CreateWidget<USelectionPointer>(World, PointerHUDClass);
                    PointerHUD->AddToViewport();
                }
            }
        }
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
