#pragma once

#include "CoreMinimal.h"

// Some utility functions.
class PACMAN_API PacmanUtilities {

public:
	// Tile utilities

	// Returns whether a pawn can walk on a specific tile.
	static bool IsTileWalkable(const class ATile*);


	// Moving direction utilities

	// Transforms a EMovingDirection into a vector.
	static FVector MovingDirectionToVector(enum class EMovingDirection dir);

	// Returns the opposite direction.
	static enum class EMovingDirection Opposite(enum class EMovingDirection dir);
};

using Util = PacmanUtilities;
