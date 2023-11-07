// Copyright Epic Games, Inc. All Rights Reserved.


#include "PartyBase.h"
#include "EnemyBase.h"
#include "TBAiGameModeBase.h"
#include <Kismet/GameplayStatics.h>

//constructor
ATBAiGameModeBase::ATBAiGameModeBase()
{
	CurrentState = ETurnState::StartTurn;
}
void ATBAiGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	StartTurn();
}
void ATBAiGameModeBase::StartTurn()
{
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("StartTurn"));
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
        for (AActor* PartyActor : FoundPartyActors)
        {
            APartyBase* PartyInstance = Cast<APartyBase>(PartyActor);
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
