#pragma once

#include "CoreMinimal.h"
#include "PacmanUtilities.generated.h"


USTRUCT()
struct PACMAN_API FTileIndex {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere)
	int Col;

	UPROPERTY(VisibleAnywhere)
	int Row;

	// Returns the euclidean length considering this TileIndex as a vector in 2D space.
	float Length() const;

	// Considers the TileIndex as if it was a vector, and rotates it towards the specified direction. NORTH: 0°, WEST = 90°, SOUTH = 180°, EAST = 270° counterclockwise. (e.g. <-1, 3>.Towards(WEST) = <-3, 1>)
	FTileIndex& RotateTowards(enum class EMovingDirection toDir);

	FTileIndex& operator-();
	FTileIndex& operator+=(const FTileIndex& rhs);
	FTileIndex& operator-=(FTileIndex rhs);
	FTileIndex& operator*=(float rhs);
	FTileIndex& operator/=(float rhs);
	friend FTileIndex operator+(FTileIndex lhs, const FTileIndex& rhs);
	friend FTileIndex operator-(FTileIndex lhs, const FTileIndex& rhs);
	friend FTileIndex operator*(FTileIndex lhs, float rhs);
	friend FTileIndex operator*(float lhs, FTileIndex rhs);
	friend FTileIndex operator/(FTileIndex lhs, float rhs);
	friend FTileIndex operator/(float lhs, FTileIndex rhs);

};


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


// Custom collision channel names
constexpr auto ObjectChannel_World2d = ECollisionChannel::ECC_GameTraceChannel1;
constexpr auto ObjectChannel_BoardPawns = ECollisionChannel::ECC_GameTraceChannel2;
constexpr auto ObjectChannel_Walls = ECollisionChannel::ECC_GameTraceChannel3;
