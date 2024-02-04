#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "TileFullTrigger.generated.h"


// Forward declarations
class AWalkableTile;


// Trigger responsible to notify when a BoardPawns leaves a Tile.
UCLASS(ClassGroup = Pacman)
class PACMAN_API UTileFullTrigger : public USphereComponent {
	GENERATED_BODY()
	
public:
	UTileFullTrigger();

	// Sets the location of the component based on the tile position, and the radius based on the bounding box of the mesh attached to the tile.
	void Init(const class AWalkableTile& tile);

protected:
	UFUNCTION() // Called when another component ends overlapping with this one.
	void OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex);

private:
	const AWalkableTile* Tile; // The tile this trigger is on
};
