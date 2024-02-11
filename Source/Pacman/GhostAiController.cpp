#include "GhostAiController.h"
#include "BoardPawn.h"
#include "PacmanLevelState.h"
#include "GhostPawn.h"
#include "PacmanUtilities.h"
#include "WalkableTile.h"
#include "GhostModeData.h"
#include "UObject/Class.h"


// Should be called when the ghost reaches the center of a tile; it computes where the ghost should go next and tells it to the ghost.
void AGhostAiController::GhostOnTileCenter(const AWalkableTile& tile) {
	auto levelState = Cast<APacmanLevelState>(GetWorld()->GetGameState());
	auto possessedGhost = Cast<AGhostPawn>(GetPawn());

	Target = AcquireNewTarget(*levelState, *possessedGhost);
	auto newDirection = ComputeBestDirection();
	possessedGhost->TurnDirection(newDirection);

	UE_LOG(LogTemp, Display, TEXT("GhostAI %s new target and direction are: <%i, %i> - %s"), *possessedGhost->GetName(), Target.Col, Target.Row, *UEnum::GetValueAsString<EMovingDirection>(newDirection));
}


void AGhostAiController::SetMode(const UGhostModeData& mode) {
	// Set the target acquisition function
	AcquireNewTarget = [&mode](GHOST_TARGET_ACQUISITION_PARAMS) {return mode.TargetAcquisitionClass.GetDefaultObject()->AcquireTarget(levelState, itself); };

	// If the new mode is frightened, invert direction
	if (mode.Mode == EGhostMode::FRIGHTENED) {
		auto possessedPawn = Cast<AGhostPawn>(GetPawn());
		possessedPawn->TurnDirection(Util::Opposite(possessedPawn->GetMovingDirection()));
	}
}


// Returns the best direction to get to the target tile. In this case it tries all possible surrounding tiles, and goes to the tile with the shortest distance to the target (original Pacman AI).
EMovingDirection AGhostAiController::ComputeBestDirection() const {
	auto ghost = Cast<AGhostPawn>(GetPawn());
	auto state = Cast<APacmanLevelState>(GetWorld()->GetGameState());

	auto surroundingTiles = state->GetAbstractMap().GetSurroundingTiles(*ghost);
	auto movingDir = ghost->GetMovingDirection();

	surroundingTiles.FindAndRemoveChecked(Util::Opposite(movingDir)); // The ghost cannot do a U turn
	
	// In case of equal distance, the directions must have this priority: NORTH > WEST > SOUTH > EAST
	surroundingTiles.KeySort([](EMovingDirection a, EMovingDirection b) { return a < b; });
	EMovingDirection bestDir = Util::Opposite(movingDir); // If we cannot find any other direction, the only option is to do a U turn. This should never happen on the historic map.
	for (float shortestDistance = TNumericLimits<float>::Max(); const auto & [dir, tile] : surroundingTiles) {
		if (!Util::IsTileWalkable(tile)) continue;
		float distanceToTarget = (tile->Index - Target).Length(); // Euclidean distance between the center of the new tile and the target
		if (distanceToTarget < shortestDistance) {
			shortestDistance = distanceToTarget;
			bestDir = dir;
		}

		UE_LOG(LogTemp, Display, TEXT("GhostAI %s distance to target %s = %f"), *GetPawn()->GetName(), *UEnum::GetValueAsString<EMovingDirection>(dir), distanceToTarget);
	}
	
	if (bestDir == Util::Opposite(movingDir)) UE_LOG(LogTemp, Warning, TEXT("AGhostAiController::ComputeBestDirection: Ghost forced to perform a U turn."));
	return bestDir;
}
