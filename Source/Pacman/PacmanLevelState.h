#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PacmanLevelState.generated.h"


// Forward declarations
class ATile;
class AWalkableTile;
class ABoardPawn;
class APacmanPawn;
class AGhostPawn;
class APacmanGameMode;
class IInitializable;
class IStartableStoppable;
class UAbstractMap;
class UPacmanGameInstance;
enum class EMovingDirection;
enum class ECharacterTag;


UCLASS()
class PACMAN_API APacmanLevelState : public AGameStateBase {
	GENERATED_BODY()
	
public:
	APacmanLevelState();

	// Initializes the level state. Should be called before the start of the game. We cannot use BeginPlay because the call order is not defined, and the game state must be initialized before any other actor.
	virtual void Init();

	// Starts the level by calling Start on all the actors that registered during BeginPlay.
	virtual void Start();

	// These functions are called by other actors to update the game state

	// Updates the tile where the specified BoardPawn in the AbstractMap, and returns the new tile.
	const AWalkableTile& UpdateBoardPawnTile(const ABoardPawn& pawn, const FVector& position);

	// Should be called when a standard food is eaten by Pacman.
	void NotifyStandardFoodEaten(unsigned int value);

	// Should be called when a fruit is eaten by Pacman.
	void NotifyFruitEaten(unsigned int value);

	// Should be called when a power pellet food is eaten by Pacman.
	void NotifyPowerPelletEaten(unsigned int value);

	// Should be called when a ghost is eaten by Pacman.
	void NotifyGhostEaten(AGhostPawn& ghost);

	// Should be called when a ghost eats Pacman.
	void NotifyPacmanDead();

	// Increases AvailableStandardFood.
	void AddStandardFood(unsigned int quantity = 1);

	// Adds the specified amount of points
	void AddPoints(unsigned int quantity);


	// Functions to retrieve information about the level state.

	// Returns the surrounding tiles on the map of the specified pawn.
	TMap<EMovingDirection, const ATile*> GetSurroundingTiles(const ABoardPawn& pawn) const;

	// Returns the abstract map itslef.
	const UAbstractMap& GetAbstractMap() const;

	// Returns the current Pacman
	APacmanPawn* GetPacman() const;

	// Returns the first board pawn with the specified tag (if present)
	ABoardPawn* GetBoardPawn(ECharacterTag tag) const;

	// Returns the map containing the references to the present pawns.
	const TArray<ABoardPawn*>& GetBoardPawns() const;

	// Returns the available food
	unsigned int GetAvailableFood() const;

	void RegisterForInitialization(IInitializable& actor, bool unregister = false) const;

	void RegisterForStartAndStop(IStartableStoppable& actor, bool unregister = false) const;

	void RegisterBoardPawn(ABoardPawn& boardPawn);

protected:
	// Decreases AvailableStandardFood and notify this to the game mode.
	void DecreaseStandardFood(unsigned int quantity = 1);


	UAbstractMap* Map; // Object keeping track of the tiles of the map and the position of the board pawns.
	TArray<ABoardPawn*> BoardPawns; // List of the board pawns present in the level.	
	unsigned int AvailableStandardFood; // How many items of standard food are still on the board.
	
	struct PowerPelletActivation {
		unsigned int GhostsEatenInThisPowerPellet; // How many ghosts got eaten during this power pellet activation
		FTimerHandle Countdown; // Timer
	} CurrentPowerPelletActivation;

	UPacmanGameInstance* GameInstance; // The game instance
	APacmanGameMode* GameMode; // The game mode

	mutable TSet<IInitializable*> ActorsToInitialize; // Contains the actors that registered for initialization
	mutable TSet<IStartableStoppable*> ActorsToStart; // Contains the actors that registered to receive the notification that they can start their routine

public:
	// Returns the current power pellet activation
	PowerPelletActivation& GetCurrentPowerPelletActivation();
};
