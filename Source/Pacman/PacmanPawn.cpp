#include "PacmanPawn.h"
#include "BoardPawnMovementComponent.h"
#include "WalkableTile.h"
#include "PacmanLevelState.h"
#include "AbstractMap.h"
#include "PacmanSettings.h"
#include "PacmanUtilities.h"

APacmanPawn::APacmanPawn() {
}


void APacmanPawn::Init() {
	Super::Init();
}


// Called to notify the pawn that it is at the center of the current tile
void APacmanPawn::OnTileCenter(const AWalkableTile& tile) {
	auto surroundingTiles = Cast<APacmanLevelState>(GetWorld()->GetGameState())->GetSurroundingTiles(*this);
	// If the input direction is valid, go in that direction
	if (auto nextTile = surroundingTiles[InputDirection]; nextTile && nextTile->IsA(AWalkableTile::StaticClass())) {
		MovementComponent->SetMovingDirection(InputDirection);
		MovementComponent->SetSpeed(GetActualSpeed(tile));
	}
	Super::OnTileCenter(tile);
}


// Pacman turns into the specified direction.
void APacmanPawn::TurnDirection(EMovingDirection dir) {
	InputDirection = dir;
	// If Pacman tried to do a U turn, do it, else wait until it reached the center of the tile to perform the change of direction.
	if (MovementComponent->GetMovingDirection() == PacmanUtilities::Opposite(dir)) {
		MovementComponent->SetMovingDirection(InputDirection);
	}

	// Also, if Pacman is stationary (it hit a wall and is now standing on a tile), immediately notify the change of direction.
	if (MovementComponent->IsStationary()) {
		OnTileCenter(*CurrentTile);
	}
}

