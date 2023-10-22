// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePos.h"

// Sets default values
ABattlePos::ABattlePos()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABattlePos::BeginPlay()
{
	Super::BeginPlay();
	FVector location = GetActorLocation();
	FRotator rotation = GetActorRotation();
	//GetWorld()->SpawnActor<AUnitBase>(UnitRef, location, rotation);
}

// Called every frame
void ABattlePos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
