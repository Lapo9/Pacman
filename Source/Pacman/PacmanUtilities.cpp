#include "PacmanUtilities.h"
#include "AbstractMap.h"
#include "BoardPawn.h"
#include "Tile.h"
#include "WalkableTile.h"
#include "WallTile.h"

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
		return FVector::RightVector;
		break;
	case EMovingDirection::EAST:
		return FVector::BackwardVector;
		break;
	case EMovingDirection::SOUTH:
		return FVector::LeftVector;
		break;
	case EMovingDirection::WEST:
		return FVector::ForwardVector;
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

