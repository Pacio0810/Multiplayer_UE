#include "WinArea.h"
#include "MultiplayerCharacter.h"

AWinArea::AWinArea()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	WinAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WinAreaBox"));
	SetRootComponent(WinAreaBox);
	bWinCondition = false;
}

void AWinArea::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWinArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (!bWinCondition)
		{
			TArray<AActor*> OverlapActors;
			WinAreaBox->GetOverlappingActors(OverlapActors, AMultiplayerCharacter::StaticClass());

			bWinCondition = (OverlapActors.Num() == 2);

			if (bWinCondition)
			{
				MulticastRPCWin();
			}
		}
	}
}

void AWinArea::MulticastRPCWin_Implementation()
{
	OnWinCOndition.Broadcast();
}

