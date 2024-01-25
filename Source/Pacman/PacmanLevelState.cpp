#include "PacmanLevelState.h"
#include "BoardPawn.h"
#include "AbstractMap.h"

APacmanLevelState::APacmanLevelState() {
	Map = CreateDefaultSubobject<UAbstractMap>(TEXT("Map"));
}


// Initializes the level state. Should be called before the start of the game. We cannot use BeginPlay because the call order is not defined, and the game state must be initialized before any other actor.
void APacmanLevelState::Init() {
	Map->Init(); // Fill the abstract map based on the tiles in the level
}

const UAbstractMap& APacmanLevelState::GetAbstractMap() const {
	return *Map;
}


// Returns the surrounding tiles on the map of the specified pawn.
TMap<EMovingDirection, const class ATile*> APacmanLevelState::GetSurroundingTiles(ECharacterTag tag) const {
	return Map->GetSurroundingTiles(tag);
}


// Updates the tile of a BoardPawn in the AbstractMap, and returns the new tile.
const ATile& APacmanLevelState::UpdateBoardPawnTile(ECharacterTag tag, const FVector& position) {
	return Map->UpdateCharacterTile(tag, position);
}
