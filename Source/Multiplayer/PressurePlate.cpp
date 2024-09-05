// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"

// Sets default values
APressurePlate::APressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	bActivated = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->SetupAttachment(RootComp);
	TriggerMesh->SetIsReplicated(true);

	auto TriggerMeshAssets = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Asian_town/Assets/floor_bricks/SM_floor_bricks_02.SM_floor_bricks_02"));

	if (TriggerMeshAssets.Succeeded())
	{
		TriggerMesh->SetStaticMesh(TriggerMeshAssets.Object);
		TriggerMesh->SetRelativeScale3D(FVector(3.8f, 3.8f, 1.5f));
		TriggerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);

	auto MeshAssets = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Asian_town/Assets/floor_bricks/SM_floor_bricks_02.SM_floor_bricks_02"));

	if (MeshAssets.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAssets.Object);
		Mesh->SetRelativeScale3D(FVector(4.0f, 4.0f, 0.5f));
		Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 7.2f));
	}
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerMesh->SetVisibility(false);
	TriggerMesh->SetCollisionProfileName(FName("OverlapAll"));
}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

