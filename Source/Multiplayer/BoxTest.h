// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystem.h"
#include "BoxTest.generated.h"

UCLASS()
class MULTIPLAYER_API ABoxTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoxTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Per rendere replicabile una variabile bisogna inserire replicated nelle UPROPERTY e
	// overridare un metodo, ovvero GetLifetimeReplicatedProps
	UPROPERTY(BlueprintReadWrite, Replicated)
	float ReplicatedVar;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_RepNotifyVar, BlueprintReadWrite)
	float RepNotifyVar;

	UFUNCTION(BlueprintCallable)
	void OnRep_RepNotifyVar();

	void DecreaseRepNotifyVar();
	FTimerHandle TestTimer;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCExplode();

	UPROPERTY(EditAnywhere)
	UParticleSystem* ExplosionEffect;
};
