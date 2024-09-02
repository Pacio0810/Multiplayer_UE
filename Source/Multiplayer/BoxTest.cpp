// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxTest.h"
#include "Net/UnrealNetwork.h"      // Da inserire per la macro DOREPLIFETIME

// Sets default values
ABoxTest::ABoxTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ReplicatedVar = 100.f;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ABoxTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoxTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoxTest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABoxTest, ReplicatedVar);
}

