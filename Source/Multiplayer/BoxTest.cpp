// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxTest.h"
#include "Net/UnrealNetwork.h"      // Da inserire per la macro DOREPLIFETIME

// Sets default values
ABoxTest::ABoxTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ReplicatedVar = 100.f;
	RepNotifyVar = 100.0f;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ABoxTest::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);
	
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(TestTimer, this, &ABoxTest::DecreaseRepNotifyVar, 2.0f, false);
	}
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
	DOREPLIFETIME(ABoxTest, RepNotifyVar);
}

void ABoxTest::OnRep_RepNotifyVar()
{
	if (HasAuthority())
	{	
		FVector NewLocation = GetActorLocation() + FVector(0.0f, 0.0f, 200.0f);
		SetActorLocation(NewLocation);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Server: On_Rep_RepNotifyVar"));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Client: On_Rep_RepNotifyVar"), GPlayInEditorID));

	}
}

void ABoxTest::DecreaseRepNotifyVar()
{
	if (HasAuthority())
	{
		RepNotifyVar -= 1.0f;
		OnRep_RepNotifyVar();

		if (RepNotifyVar > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(TestTimer, this, &ABoxTest::DecreaseRepNotifyVar, 2.0f, false);
		}
	}
}
