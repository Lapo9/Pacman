#include "PacmanLevelState.h"
#include "BoardPawn.h"
#include "GhostPawn.h"
#include "PacmanPawn.h"
#include "AbstractMap.h"
#include "PacmanGameInstance.h"
#include "PacmanGameMode.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "UObject/Class.h"

APacmanLevelState::APacmanLevelState() {
	Map = CreateDefaultSubobject<UAbstractMap>(TEXT("Map"));

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
}


// Initializes the level state. Should be called before the start of the game. We cannot use BeginPlay because the call order is not defined, and the game state must be initialized before any other actor.
void APacmanLevelState::Init() {
	UE_LOG(LogTemp, Display, TEXT("Initializing Pacman level state..."));

	// Get the game instance and mode for faster access
	GameInstance = Cast<UPacmanGameInstance>(GetWorld()->GetGameInstance());
	GameMode = Cast<APacmanGameMode>(GetWorld()->GetAuthGameMode());

	AvailableStandardFood = 0;

	Map->Init(); // Fill the abstract map based on the tiles in the level

	// Initialize all the actors that registered during BeginPlay
	for (auto toInitialize : ActorsToInitialize) toInitialize->Init();
}


// Starts the level by calling Start on all the actors that registered during BeginPlay.
void APacmanLevelState::Start() {
	UE_LOG(LogTemp, Display, TEXT("Starting Pacman level state..."))
	Audio->Sound = StartSound;
	Audio->Play();
	for (auto toStart : ActorsToStart) toStart->Start();
}


// Should be called when a standard food is eaten by Pacman.
void APacmanLevelState::NotifyStandardFoodEaten(unsigned int value) {
	AddPoints(value);
	DecreaseStandardFood(); // Decreases the available food by 1 and notifies the game mode.
	UE_LOG(LogTemp, Display, TEXT("Standard food eaten --> Remaining food: %i"), AvailableStandardFood);
}


// Should be called when a fruit is eaten by Pacman.
void APacmanLevelState::NotifyFruitEaten(unsigned int value) {
	AddPoints(value);
	UE_LOG(LogTemp, Display, TEXT("Fruit eaten --> Remaining food: %i"), AvailableStandardFood);
}


// Should be called when a power pellet food is eaten by Pacman.
void APacmanLevelState::NotifyPowerPelletEaten(unsigned int value) {
	AddPoints(value);
	CurrentPowerPelletActivation.GhostsEatenInThisPowerPellet = 0; // Reset count
	GameMode->NotifyPowerPelletEaten();
	UE_LOG(LogTemp, Display, TEXT("Power pellet eaten --> Remaining food: %i"), AvailableStandardFood);
}


// Should be called when a ghost is eaten by Pacman.
void APacmanLevelState::NotifyGhostEaten(AGhostPawn& ghost) {
	CurrentPowerPelletActivation.GhostsEatenInThisPowerPellet++;
	GameMode->NotifyGhostEaten(ghost);

	UE_LOG(LogTemp, Display, TEXT("Ghost %s eaten --> Remaining food: %i"), *ghost.GetName(), AvailableStandardFood);
}


void APacmanLevelState::NotifyPacmanDead() {
	if (--GameInstance->Lives == 0) {
		GameMode->NotifyGameOver();
	}
	else {
		GameMode->NotifyPacmanDead();
	}
}


// Increases AvailableStandardFood.
void APacmanLevelState::AddStandardFood(unsigned int quantity) {
	AvailableStandardFood += quantity;
}


// Adds the specified amount of points
void APacmanLevelState::AddPoints(unsigned int quantity) {
	int total = GameInstance->AddPoints(quantity);
	GameMode->NotifyPointsIncreased(total, quantity); // Must be done because it is possible to gain extra lives on certain thresholds
}


// Decreases AvailableStandardFood and notify this to the game mode.
void APacmanLevelState::DecreaseStandardFood(unsigned int quantity) {
	AvailableStandardFood -= quantity;
	GameMode->NotifyAvailableFoodDecreasedBy1(AvailableStandardFood); // Used mainly by the UTimeModeManager to spawn the fruits, and to notify Blinky for elroy mode
	if (AvailableStandardFood <= 0) GameMode->NotifyLevelCleared();
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


// Returns the available food
unsigned int APacmanLevelState::GetAvailableFood() const {
	return AvailableStandardFood;
}


void APacmanLevelState::RegisterForInitialization(IInitializable& actor, bool unregister) const {
	if (unregister)	ActorsToInitialize.Remove(&actor);
	else ActorsToInitialize.Add(&actor);
}


void APacmanLevelState::RegisterForStartAndStop(IStartableStoppable& actor, bool unregister) const {
	if (unregister) ActorsToStart.Remove(&actor);
	else ActorsToStart.Add(&actor);
}


void APacmanLevelState::RegisterBoardPawn(ABoardPawn& boardPawn) {
	BoardPawns.Add(&boardPawn);
	UE_LOG(LogTemp, Display, TEXT("Board pawn registered to level state: %s - %s"), *UEnum::GetValueAsString<ECharacterTag>(boardPawn.GetTag()), *boardPawn.GetName());
}


APacmanLevelState::PowerPelletActivation& APacmanLevelState::GetCurrentPowerPelletActivation() {
	return CurrentPowerPelletActivation;
}


// Returns the first board pawn with the specified tag (if present)
ABoardPawn* APacmanLevelState::GetBoardPawn(ECharacterTag tag) const {
	return *BoardPawns.FindByPredicate([tag](auto pawn) {return pawn->GetTag() == tag; });
}


// Updates the tile of a BoardPawn in the AbstractMap, and returns the new tile.
const AWalkableTile* APacmanLevelState::UpdateBoardPawnTile(const ABoardPawn& pawn, const FVector& position) {
	return Map->UpdateCharacterTile(pawn, position);
}
