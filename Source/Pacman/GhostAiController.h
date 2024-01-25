#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GhostAiController.generated.h"


// Decides how a ghost should move.
UCLASS()
class PACMAN_API AGhostAiController : public AAIController
{
	GENERATED_BODY()
	
public:
	// Initializes the AI by selecting a target acquisition function.
	virtual void Init(TFunction<const class ATile* (const class UAbstractMap&)> acquireNewTarget);

	// Should be called when the ghost reaches the center of a tile; it computes where the ghost should go next and tells it to the ghost.
	virtual void GhostOnTileCenter(const class ATile& tile);

protected:
	// Returns the best direction to get to the target tile.
	virtual enum class EMovingDirection ComputeBestDirection() const;

	// Funtion to decide the target.
	TFunction<const class ATile*(const class UAbstractMap&)> AcquireNewTarget;

	UPROPERTY(VisibleAnywhere, Category = "Real-time info") // Tile the ghost aims to.
	const class ATile* Target;
};
