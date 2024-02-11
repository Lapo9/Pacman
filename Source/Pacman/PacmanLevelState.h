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
class UAudioComponent;
class USoundBase;
class UPacmanGameInstance;
enum class EMovingDirection;
enum class ECharacterTag;


// APacmanLevelState is responsible to get updates from the game actors (e.g. a food eaten, Pacman's death, ...), store the data of the event (e.g. +1 points, ...) and notify the event to APacmanGameMode (or to the game instance).
// It is also responsible to initialize and send the start event to the registered actors.
// Most of the data of APacmanLevelState can be queried with the approprieate functions (generally it is queried by the APacmanGameMode).
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
	const AWalkableTile* UpdateBoardPawnTile(const ABoardPawn& pawn, const FVector& position);

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

	// Adds the component to the ones to send an initialization event to.
	void RegisterForInitialization(IInitializable& actor, bool unregister = false) const;

	// Adds the component to the ones to send a start event to.
	void RegisterForStartAndStop(IStartableStoppable& actor, bool unregister = false) const;

	void RegisterBoardPawn(ABoardPawn& boardPawn);

protected:
	// Decreases AvailableStandardFood and notify this to the game mode.
	void DecreaseStandardFood(unsigned int quantity = 1);

	UAudioComponent* Audio; // Used to play level sounds

	UPROPERTY(EditAnywhere, Category = "Pacman|Sounds") // Sound to play at level start
	USoundBase* StartSound;

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
