#pragma once

#include "CoreMinimal.h"
#include "BaseFood.h"
#include "PowerPelletFood.generated.h"


// Power pellets that turn ghosts into frightened mode for a given amount of seconds when eaten.
UCLASS()
class PACMAN_API APowerPelletFood : public ABaseFood {
	GENERATED_BODY()

protected:
	void BeginPlay() override;

	// Called when another component starts overlapping with this one.
	void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) override;
};
