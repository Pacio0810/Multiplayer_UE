#include "CollectableKey.h"
#include "Net/UnrealNetwork.h"

ACollectableKey::ACollectableKey()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(RootComp);
	CapsuleComponent->SetIsReplicated(true);
	CapsuleComponent->SetCollisionProfileName(FName("OverlapAllDynamic"));
	CapsuleComponent->SetCapsuleHalfHeight(150.0f);
	CapsuleComponent->SetCapsuleRadius(100.0f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);
	Mesh->SetCollisionProfileName(FName("OverlapAllDynamic"));

}

void ACollectableKey::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACollectableKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectableKey::OnRep_IsCollected()
{

}

void ACollectableKey::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifeTimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifeTimeProps);

	DOREPLIFETIME(ACollectableKey, IsCollected);
}