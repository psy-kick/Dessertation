// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include <Kismet/GameplayStatics.h>

AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
int AEnemyBase::CalculateTotalEnemyMP()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::GetAllActorsOfClass(World, AEnemyBase::StaticClass(), FoundActors);
		for (AActor* enemy : FoundActors)
		{
			AEnemyBase* EnemyInstance = Cast<AEnemyBase>(enemy);
			if (EnemyInstance)
			{
				TotalEnemyMp += EnemyInstance->MP;
			}
		}
	}
	return TotalEnemyMp;
}
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
