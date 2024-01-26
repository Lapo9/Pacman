#include "PacmanLevelState.h"
#include "BoardPawn.h"
#include "PacmanPawn.h"
#include "AbstractMap.h"
#include "UObject/Class.h"

APacmanLevelState::APacmanLevelState() {
	Map = CreateDefaultSubobject<UAbstractMap>(TEXT("Map"));
}


// Initializes the level state. Should be called before the start of the game. We cannot use BeginPlay because the call order is not defined, and the game state must be initialized before any other actor.
void APacmanLevelState::Init() {
	UE_LOG(LogTemp, Display, TEXT("Initializing Pacman level state..."));
	// Set the board pawn present in this level
	BoardPawns = TMap<ECharacterTag, ABoardPawn*>{};
	for (TObjectIterator<ABoardPawn> pawn; pawn; ++pawn) {
		BoardPawns.Add(pawn->GetTag(), *pawn);
		UE_LOG(LogTemp, Display, TEXT("Pacman level board pawn added: %s - %s"), *UEnum::GetValueAsString<ECharacterTag>(pawn->GetTag()), *pawn->GetName());
	}

	Map->Init(); // Fill the abstract map based on the tiles in the level
}


const UAbstractMap& APacmanLevelState::GetAbstractMap() const {
	return *Map;
}


APacmanPawn* APacmanLevelState::GetPacman() {
	return Cast<APacmanPawn>(BoardPawns[ECharacterTag::PACMAN]);
}


// Returns the map containing the references to the present pawns.
const TMap<enum class ECharacterTag, class ABoardPawn*>& APacmanLevelState::GetBoardPawns() const {
	return BoardPawns;
}


ABoardPawn* APacmanLevelState::GetBoardPawn(ECharacterTag tag) {
	return BoardPawns[tag];
}


// Returns the surrounding tiles on the map of the specified pawn.
TMap<EMovingDirection, const class ATile*> APacmanLevelState::GetSurroundingTiles(ECharacterTag tag) const {
	return Map->GetSurroundingTiles(tag);
}


// Updates the tile of a BoardPawn in the AbstractMap, and returns the new tile.
const ATile& APacmanLevelState::UpdateBoardPawnTile(ECharacterTag tag, const FVector& position) {
	return Map->UpdateCharacterTile(tag, position);
}
