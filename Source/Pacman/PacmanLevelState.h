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
	const class ATile& UpdateBoardPawnTile(enum class ECharacterTag tag, const FVector& position);

	// Should be called when a standard food is eaten by Pacman.
	void NotifyStandardFoodEaten(unsigned int value);

	// Should be called when a fruit is eaten by Pacman.
	void NotifyFruitEaten(unsigned int value);

	// Should be called when a power pellet food is eaten by Pacman.
	void NotifyPowerPelletEaten(unsigned int value);

	// Should be called when a ghost is eaten by Pacman.
	void NotifyGhostEaten();

	// Increases AvailableStandardFood.
	void AddStandardFood(unsigned int quantity = 1);


	// Functions to retrieve information about the level state.

	// Returns the surrounding tiles on the map of the specified pawn.
	TMap<enum class EMovingDirection, const class ATile*> GetSurroundingTiles(enum class ECharacterTag tag) const;

	// Returns the abstract map itslef.
	const class UAbstractMap& GetAbstractMap() const;

	// Returns the current pacman
	class APacmanPawn* GetPacman();

	// Returns the specified board pawn
	class ABoardPawn* GetBoardPawn(enum class ECharacterTag tag);

	// Returns the map containing the references to the present pawns.
	const TMap<enum class ECharacterTag, class ABoardPawn*>& GetBoardPawns() const;

protected:
	// Decreases AvailableStandardFood and notify this to the game mode.
	void DecreaseStandardFood(unsigned int quantity = 1);


	// Object keeping track of the tiles of the map and the position of the board pawns.
	class UAbstractMap* Map;

	// List of the board pawns present in the level.
	class TMap<enum class ECharacterTag, class ABoardPawn*> BoardPawns;

	// Points till now.
	unsigned int Points;

	// How many items of standard food are still on the board.
	unsigned int AvailableStandardFood;
};
