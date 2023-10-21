// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoints.h"

// Sets default values
ASpawnPoints::ASpawnPoints()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnPoints::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnPoints::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

