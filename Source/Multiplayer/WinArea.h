#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "WinArea.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWinAreaCondition);

UCLASS()
class MULTIPLAYER_API AWinArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AWinArea();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* WinAreaBox;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bWinCondition;

	UPROPERTY(BlueprintAssignable)
	FOnWinAreaCondition OnWinCOndition;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCWin();
};
