#pragma once

#include "CoreMinimal.h"
#include "PacmanUtilities.h"
#include "AIController.h"
#include "GhostAiController.generated.h"


using GhostAcquireNewTargetSignature = struct FTileIndex(const class UAbstractMap& map, const class APacmanPawn& pacman, const class ABoardPawn& itself, const TMap<enum class ECharacterTag, class ABoardPawn*>& otherPawns);

// Decides how a ghost should move.
UCLASS()
class PACMAN_API AGhostAiController : public AAIController {
	GENERATED_BODY()

public:
	// Initializes the AI by selecting a target acquisition function.
	virtual void Init(TFunction<GhostAcquireNewTargetSignature> acquireNewTarget);

	// Should be called when the ghost reaches the center of a tile; it computes where the ghost should go next and tells it to the ghost.
	virtual void GhostOnTileCenter(const class ATile& tile);

protected:
	// Returns the best direction to get to the target tile. In this case it tries all possible surrounding tiles, and goes to the tile with the shortest distance to the target (original Pacman AI).
	virtual enum class EMovingDirection ComputeBestDirection() const;

	// Funtion to decide the target.
	TFunction<GhostAcquireNewTargetSignature> AcquireNewTarget;

	UPROPERTY(VisibleAnywhere, Category = "Real-time info") // Tile the ghost aims to.
	FTileIndex Target;
};

