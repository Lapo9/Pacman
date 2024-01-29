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
	BoardPawns = TArray<ABoardPawn*>{};
	for (TObjectIterator<ABoardPawn> pawn; pawn; ++pawn) {
		if (pawn->GetWorld() != GetWorld()) continue; // Skip objects that are not in the actual scene (maybe they were in the editor or somewhere else)
		BoardPawns.Add(*pawn);
		UE_LOG(LogTemp, Display, TEXT("Pacman level board pawn added: %s - %s"), *UEnum::GetValueAsString<ECharacterTag>(pawn->GetTag()), *pawn->GetName());
	}

	Map->Init(); // Fill the abstract map based on the tiles in the level
}


// Should be called when a standard food is eaten by Pacman.
void APacmanLevelState::NotifyStandardFoodEaten(unsigned int value) {
	Points += value;
	DecreaseStandardFood(); // Decreases the available food by 1 and notifies the game mode.
	UE_LOG(LogTemp, Display, TEXT("Standard food eaten --> Points: %i - Remaining food: %i"), Points, AvailableStandardFood);
}


// Should be called when a fruit is eaten by Pacman.
void APacmanLevelState::NotifyFruitEaten(unsigned int value) {
	Points += value;
	UE_LOG(LogTemp, Display, TEXT("Fruit eaten --> Points: %i - Remaining food: %i"), Points, AvailableStandardFood);
}


// Should be called when a power pellet food is eaten by Pacman.
void APacmanLevelState::NotifyPowerPelletEaten(unsigned int value) {
	// TODO
}


// Should be called when a ghost is eaten by Pacman.
void APacmanLevelState::NotifyGhostEaten() {
	// TODO
}


// Increases AvailableStandardFood.
void APacmanLevelState::AddStandardFood(unsigned int quantity) {
	AvailableStandardFood += quantity;
}


// Decreases AvailableStandardFood and notify this to the game mode.
void APacmanLevelState::DecreaseStandardFood(unsigned int quantity) {
	AvailableStandardFood -= quantity;
	// TODO notify game mode
}


const UAbstractMap& APacmanLevelState::GetAbstractMap() const {
	return *Map;
}


// Returns the current Pacman
APacmanPawn* APacmanLevelState::GetPacman() const {
	auto pacman = GetBoardPawn(ECharacterTag::PACMAN);
	return pacman ? Cast<APacmanPawn>(pacman) : nullptr;
}


// Returns the map containing the references to the present pawns.
const TArray<ABoardPawn*>& APacmanLevelState::GetBoardPawns() const {
	return BoardPawns;
}


// Returns the first board pawn with the specified tag (if present)
ABoardPawn* APacmanLevelState::GetBoardPawn(ECharacterTag tag) const {
	return *BoardPawns.FindByPredicate([tag](auto pawn) {return pawn->GetTag() == tag; });
}


// Returns the surrounding tiles on the map of the specified pawn.
TMap<EMovingDirection, const class ATile*> APacmanLevelState::GetSurroundingTiles(const ABoardPawn& pawn) const {
	return Map->GetSurroundingTiles(pawn);
}


// Updates the tile of a BoardPawn in the AbstractMap, and returns the new tile.
const AWalkableTile& APacmanLevelState::UpdateBoardPawnTile(const ABoardPawn& pawn, const FVector& position) {
	return Map->UpdateCharacterTile(pawn, position);
}
