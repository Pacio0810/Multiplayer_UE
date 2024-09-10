#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "CollectableKey.generated.h"

UCLASS()
class MULTIPLAYER_API ACollectableKey : public AActor
{
	GENERATED_BODY()

public:
	ACollectableKey();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RotationSpeed;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UAudioComponent* CollectAudio;

	UPROPERTY(ReplicatedUsing = OnRep_IsCollected, BlueprintReadWrite, VisibleAnywhere)
	bool IsCollected;

	UFUNCTION()
	void OnRep_IsCollected();

};
