// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyBase.h"
#include <Kismet/GameplayStatics.h>
#include "Components/WidgetComponent.h"
#include "EnemyBase.h"
#include "SelectionPointer.h"

// Sets default values
APartyBase::APartyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentState = EPlayerStates::SelectHero;
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponents"));
}

int APartyBase::CalculateTotalPartyMP()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::GetAllActorsOfClass(World, APartyBase::StaticClass(), FoundActors);
		for (AActor* party : FoundActors)
		{
			APartyBase* PartyInstance = Cast<APartyBase>(party);
			if (PartyInstance)
			{
				TotalPartyMp += PartyInstance->MP;
			}
		}
	}
	return TotalPartyMp;
}

void APartyBase::SelectHero()
{
	
}
void APartyBase::AttackEnemy()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::GetAllActorsOfClass(World, AEnemyBase::StaticClass(), FoundEnemies);
		if (FoundEnemies.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, FoundEnemies.Num() - 1);
			AEnemyBase* RandomEnemy = Cast<AEnemyBase>(FoundEnemies[RandomIndex]);
			if (RandomEnemy)
			{
				if (HeavyAttackFlag == true)
				{
					HeavyAttack();
				}
				else if (LightAttackFlag == true)
				{
					LightAttack();
				}
			}
		}
	}
}
void APartyBase::HeavyAttack()
{
	UE_LOG(LogTemp, Error, TEXT("HeavyAttack."));
}
void APartyBase::LightAttack()
{
	UE_LOG(LogTemp, Error, TEXT("LightAttack."));
}
// Called when the game starts or when spawned
void APartyBase::BeginPlay()
{
	Super::BeginPlay();
	if (CurrentState == EPlayerStates::SelectHero)
	{
		SelectHero();
	}
}

// Called every frame
void APartyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APartyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

