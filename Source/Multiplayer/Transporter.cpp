#include "Transporter.h"
#include "PressurePlate.h"
#include "CollectableKey.h"

UTransporter::UTransporter()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	MoveTime = 3.0f;
	ActivatedTriggerCount = 0;
	ArePointsSet = false;
	StartPoint = FVector::Zero();
	EndPoint = FVector::Zero();
}

void UTransporter::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* TriggerActor : TriggerActors)
	{
		APressurePlate* PressurePlate = Cast<APressurePlate>(TriggerActor);

		if (PressurePlate)
		{
			PressurePlate->OnActivated.AddDynamic(this, &UTransporter::OnTriggerActorActivated);
			PressurePlate->OnDeactivated.AddDynamic(this, &UTransporter::OnTriggerActorDeactivated);
			continue;
		}

		ACollectableKey* KeyActor = Cast<ACollectableKey>(TriggerActor);
		if (KeyActor)
		{
			KeyActor->OnCollected.AddDynamic(this, &UTransporter::OnTriggerActorActivated);
		}
	}
}

void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TriggerActors.Num() > 0)
	{
		if (ActivatedTriggerCount >= TriggerActors.Num())
		{
			AllTriggerActorsTriggered = true;
		}
		else
		{
			AllTriggerActorsTriggered = false;
		}
	}

	AActor* MyOwner = GetOwner();

	if (MyOwner && MyOwner->HasAuthority() && ArePointsSet)
	{
		FVector CurrentLocation = MyOwner->GetActorLocation();
		float Speed = FVector::Distance(StartPoint, EndPoint) / MoveTime;

		FVector TargetLocation = AllTriggerActorsTriggered ? EndPoint : StartPoint;

		if (!CurrentLocation.Equals(TargetLocation))
		{
			FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
			MyOwner->SetActorLocation(NewLocation);
		}
	}
}

void UTransporter::SetPoints(FVector Point1, FVector Point2)
{
	if (Point1.Equals(Point2))
	{
		return;
	}

	StartPoint = Point1;
	EndPoint = Point2;
	ArePointsSet = true;
}

void UTransporter::OnTriggerActorActivated()
{
	ActivatedTriggerCount++;
}

void UTransporter::OnTriggerActorDeactivated()
{
	ActivatedTriggerCount--;
}