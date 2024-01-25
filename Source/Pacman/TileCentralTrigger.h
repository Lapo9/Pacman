#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "TileCentralTrigger.generated.h"

// Trigger responsible to detect when a BoardPawn has reached the center of the Tile.
UCLASS(ClassGroup = Pacman)
class PACMAN_API UTileCentralTrigger : public USphereComponent {
	GENERATED_BODY()
	
public:
	UTileCentralTrigger();

	// Sets the location of the component based on the tile position, and the radius of the sphere trigger.
	void Init(const class AWalkableTile& tile);

protected:
	UFUNCTION()	// Called when another component starts overlapping with this one.
	void OnBeginOverlap(class UPrimitiveComponent* overlappedComponent, class AActor* otherActor, class UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);

	UFUNCTION() // Called when another component ends overlapping with this one.
	void OnEndOverlap(class UPrimitiveComponent* overlappedComponent, class AActor* otherActor, class UPrimitiveComponent* otherComponent, int32 otherBodyIndex);

private:
	const class AWalkableTile* Tile; // The tile this trigger is on
};
