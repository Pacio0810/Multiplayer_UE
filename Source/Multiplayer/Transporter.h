// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Transporter.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYER_API UTransporter : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTransporter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector StartPoint;
	FVector EndPoint;

	bool ArePointsSet;

	UPROPERTY(EditAnywhere)
	float MoveTime;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> TriggerActors;

	UPROPERTY(visibleAnywhere)
	int32 ActivatedTriggerCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool AllTriggerActorsTriggered;

	UFUNCTION(BlueprintCallable)
	void SetPoints(FVector Point1, FVector Point2);

	UFUNCTION()
	void OnPressurePlateActivated();

	UFUNCTION()
	void OnPressurePlateDeactivated();
};
