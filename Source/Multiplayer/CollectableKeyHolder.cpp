#include "CollectableKeyHolder.h"

ACollectableKeyHolder::ACollectableKeyHolder()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	
	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	KeyMesh->SetupAttachment(RootComp);
	KeyMesh->SetIsReplicated(true);
	KeyMesh->SetCollisionProfileName(FName("OverlapAllDynamic"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);
	Mesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	KeyMeshRotationSpeed = 100.0f;
}

void ACollectableKeyHolder::BeginPlay()
{
	Super::BeginPlay();
	
	KeyMesh->SetVisibility(false);
}

void ACollectableKeyHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	KeyMesh->AddRelativeRotation(FRotator(0.0f, KeyMeshRotationSpeed * DeltaTime, 0.0f));
}

void ACollectableKeyHolder::ActivateKeyMesh()
{
	KeyMesh->SetVisibility(true);
}

