// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyBase.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
APartyBase::APartyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentState = EPlayerState::SelectHero;
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

// Called when the game starts or when spawned
void APartyBase::BeginPlay()
{
	Super::BeginPlay();
	if (CurrentState == EPlayerState::SelectHero)
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

