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

	// Returns the surrounding tiles on the map of the specified pawn.
	TMap<enum class EMovingDirection, const class ATile*> GetSurroundingTiles(enum class ECharacterTag tag) const;

	// Returns the abstract map itslef.
	const class UAbstractMap& GetAbstractMap() const;

private:
	class UAbstractMap* Map;
};
