#include "PacmanUtilities.h"
#include "AbstractMap.h"
#include "BoardPawn.h"
#include "Tile.h"
#include "WalkableTile.h"
#include "WallTile.h"
#include "GenericPlatform/GenericPlatformMath.h"

// Returns whether a pawn can walk on a specific tile.
bool PacmanUtilities::IsTileWalkable(const ATile* tile) {
	if (tile == nullptr) return false; // If the tile is not initialized, we consider it as a wall
	switch (tile->GetType()) {
	case ETileType::WALKABLE:
		return true;
		break;
	case ETileType::WALL:
		return false;
		break;
	case ETileType::TUNNEL:
		return true;
		break;
	case ETileType::TELEPORT:
		return true;
		break;
	case ETileType::HUT:
		return false;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("PacmanUtilities::IsTileWalkable: unknown ETileType."));
		return false;
		break;
	}
}


// Transforms a EMovingDirection into a vector.
FVector PacmanUtilities::MovingDirectionToVector(EMovingDirection dir) {
	switch (dir) {
	case EMovingDirection::NORTH:
		return FVector::ForwardVector;
		break;
	case EMovingDirection::EAST:
		return FVector::RightVector;
		break;
	case EMovingDirection::SOUTH:
		return FVector::BackwardVector;
		break;
	case EMovingDirection::WEST:
		return FVector::LeftVector;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("PacmanUtilities::MovingDirectionToVector: unknown EMovingDirection."));
		return FVector::Zero();
		break;
	}
}


// Returns the opposite direction.
EMovingDirection PacmanUtilities::Opposite(EMovingDirection dir) {
	switch (dir) {
	case EMovingDirection::NORTH:
		return EMovingDirection::SOUTH;
		break;
	case EMovingDirection::EAST:
		return EMovingDirection::WEST;
		break;
	case EMovingDirection::SOUTH:
		return EMovingDirection::NORTH;
		break;
	case EMovingDirection::WEST:
		return EMovingDirection::EAST;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("PacmanUtilities::Opposite: Unknown EMovingDirection."));
		return EMovingDirection::NORTH;
		break;
	}
}


// Returns the euclidean length considering this FTileIndex as a vector in 2D space.
float FTileIndex::Length() const {
	return FMath::Sqrt((float)Col * Col + Row * Row);
}


// Considers the FTileIndex as if it was a vector, and rotates it towards the specified direction. NORTH: 0°, WEST = 90°, SOUTH = 180°, EAST = 270° counterclockwise. (e.g. <-1, 3>.Towards(WEST) = <-3, 1>)
FTileIndex& FTileIndex::RotateTowards(EMovingDirection toDir) {
	switch (toDir) {
	case EMovingDirection::WEST:
		Col = -Row;
		Row = Col;
		break;
	case EMovingDirection::SOUTH:
		Col = -Col;
		Row = -Row;
		break;
	case EMovingDirection::EAST:
		Col = Row;
		Row = -Col;
		break;
	}

	return *this;
}

FTileIndex& FTileIndex::operator-() {
	Col = -Col;
	Row = -Row;
	return *this;
}

FTileIndex& FTileIndex::operator+=(const FTileIndex& rhs) {
	Col += rhs.Col;
	Row += rhs.Row;
	return *this;
}

FTileIndex& FTileIndex::operator-=(FTileIndex rhs) {
	return *this += -rhs;
}

FTileIndex& FTileIndex::operator*=(float rhs) {
	Col *= rhs;
	Row *= rhs;
	return *this;
}

FTileIndex& FTileIndex::operator/=(float rhs) {
	Col /= rhs;
	Row /= rhs;
	return *this;
}

FTileIndex operator+(FTileIndex lhs, const FTileIndex& rhs) {
	return lhs += rhs;
}

FTileIndex operator-(FTileIndex lhs, const FTileIndex& rhs) {
	return lhs -= rhs;
}

FTileIndex operator*(FTileIndex lhs, float rhs) {
	return lhs *= rhs;
}

FTileIndex operator*(float lhs, FTileIndex rhs) {
	return rhs *= lhs;
}

FTileIndex operator/(FTileIndex lhs, float rhs) {
	return lhs /= rhs;
}

FTileIndex operator/(float lhs, FTileIndex rhs) {
	return rhs /= lhs;
}



