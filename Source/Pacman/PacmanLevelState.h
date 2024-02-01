#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PacmanLevelState.generated.h"


UCLASS()
class PACMAN_API APacmanLevelState : public AGameStateBase {
	GENERATED_BODY()
	
public:
	APacmanLevelState();

	// Initializes the level state. Should be called before the start of the game. We cannot use BeginPlay because the call order is not defined, and the game state must be initialized before any other actor.
	void Init();

	// These functions are called by other actors to update the game state

	// Updates the tile where the specified BoardPawn in the AbstractMap, and returns the new tile.
	const class AWalkableTile& UpdateBoardPawnTile(const class ABoardPawn& pawn, const FVector& position);

	// Should be called when a standard food is eaten by Pacman.
	void NotifyStandardFoodEaten(unsigned int value);

	// Should be called when a fruit is eaten by Pacman.
	void NotifyFruitEaten(unsigned int value);

	// Should be called when a power pellet food is eaten by Pacman.
	void NotifyPowerPelletEaten(unsigned int value);

	// Should be called when a ghost is eaten by Pacman.
	void NotifyGhostEaten(class AGhostPawn& ghost);

	// Should be called when a ghost eats Pacman.
	void NotifyPacmanDead();

	// Increases AvailableStandardFood.
	void AddStandardFood(unsigned int quantity = 1);

	// Adds the specified amount of points
	void AddPoints(unsigned int quantity);


	// Functions to retrieve information about the level state.

	// Returns the surrounding tiles on the map of the specified pawn.
	TMap<enum class EMovingDirection, const class ATile*> GetSurroundingTiles(const class ABoardPawn& pawn) const;

	// Returns the abstract map itslef.
	const class UAbstractMap& GetAbstractMap() const;

	// Returns the current Pacman
	class APacmanPawn* GetPacman() const;

	// Returns the first board pawn with the specified tag (if present)
	class ABoardPawn* GetBoardPawn(enum class ECharacterTag tag) const;

	// Returns the map containing the references to the present pawns.
	const TArray<class ABoardPawn*>& GetBoardPawns() const;

	// Returns the available food
	unsigned int GetAvailableFood() const;

protected:
	// Decreases AvailableStandardFood and notify this to the game mode.
	void DecreaseStandardFood(unsigned int quantity = 1);


	class UAbstractMap* Map; // Object keeping track of the tiles of the map and the position of the board pawns.
	class TArray<class ABoardPawn*> BoardPawns; // List of the board pawns present in the level.	
	unsigned int AvailableStandardFood; // How many items of standard food are still on the board.
	
	struct PowerPelletActivation {
		unsigned int GhostsEatenInThisPowerPellet; // How many ghosts got eaten during this power pellet activation
		FTimerHandle Countdown; // Timer
	} CurrentPowerPelletActivation;

	class UPacmanGameInstance* GameInstance; // The game instance
	class APacmanGameMode* GameMode; // The game mode

public:
	// Returns the current power pellet activation
	PowerPelletActivation& GetCurrentPowerPelletActivation();
};
