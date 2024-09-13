#include "PressurePlate.h"

APressurePlate::APressurePlate()
{
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
		TriggerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f));
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

	// si fa il check sull'authority per farlo controllare solo al server

	if (HasAuthority())
	{
		TArray<AActor*> OverlappingActors;
		AActor* TriggerActor = nullptr;
		TriggerMesh->GetOverlappingActors(OverlappingActors);

		for (int32 ActorId = 0; ActorId < OverlappingActors.Num(); ActorId++)
		{
			AActor* Actor = OverlappingActors[ActorId];
			if (Actor->ActorHasTag("TriggerActor"))
			{
				TriggerActor = Actor;
				break;
			}
		}

		if (TriggerActor)
		{
			if (!bActivated)
			{
				bActivated = true;
				OnActivated.Broadcast();
			}
		}
		else
		{
			if (bActivated)
			{
				bActivated = false;
				OnDeactivated.Broadcast();
			}
		}
	}
}

