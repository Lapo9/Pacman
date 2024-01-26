#include "PacmanPawn.h"
#include "BoardPawnMovementComponent.h"
#include "Tile.h"
#include "PacmanLevelState.h"
#include "AbstractMap.h"
#include "PacmanSettings.h"
#include "PacmanUtilities.h"

APacmanPawn::APacmanPawn() {
}


// Called to notify the pawn that it is at the center of the current tile
void APacmanPawn::OnTileCenter(const ATile& tile) {
	UE_LOG(LogTemp, Display, TEXT("Pacman on tile center %s"), *tile.GetName());

	auto surroundingTiles = Cast<APacmanLevelState>(GetWorld()->GetGameState())->GetSurroundingTiles(Tag);
	// If the input direction is valid, go in that direction
	if (PacmanUtilities::IsTileWalkable(surroundingTiles[InputDirection])) {
		MovementComponent->SetSpeed(tile.GetType() == ETileType::TUNNEL ? TunnelSpeed : StandardSpeed); // Set the speed of Pacman based on the tile he is on
		MovementComponent->SetMovingDirection(InputDirection);
	}
	// Notify to the movement component that the pawn reached the center of the tile
	MovementComponent->OnTileCenter(tile);
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



void APacmanPawn::BeginPlay() {
	Super::BeginPlay();
}
