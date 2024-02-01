#pragma once

#include "CoreMinimal.h"
#include "BaseFood.h"
#include "FruitFood.generated.h"


// A FruitFood is a food item that appears on th maze for a given amount of time when a certain threshold of eaten food is reached.
// The spawning of a FruitFood is managed by the TimeModeManager.
UCLASS()
class PACMAN_API AFruitFood : public ABaseFood {
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	// Called when another component starts overlapping with this one.
	void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) override;


	UPROPERTY(EditAnywhere, Category = "Pacman|Point system", meta = (ClampMin = "1", ClampMax = "30"))
	unsigned int Duration;

	FTimerHandle Timer;
};
