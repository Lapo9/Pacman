#pragma once

#include "CoreMinimal.h"
#include "PacmanUtilities.h"
#include "Components/ActorComponent.h"
#include "AbstractMap.generated.h"


// The types a tile can be.
UENUM()
enum class ETileType { WALKABLE, WALL, TUNNEL, TELEPORT, HUT };


UCLASS(ClassGroup=(Pacman), meta=(BlueprintSpawnableComponent))
class PACMAN_API UAbstractMap : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbstractMap();

	// Initializes the component by creating the abstract map and filling the characters starting positions.
	void Init();

	// Returns the index of the tile the queried character is on.
	FTileIndex GetCharacterTileIndex(const class ABoardPawn& pawn) const;

	// Returns a constant reference to the tile the queried character is on.
	const class AWalkableTile& GetCharacterTile(const class ABoardPawn& pawn) const;

	// Returns the surrounding tiles of a character.
	TMap<enum class EMovingDirection, const class ATile*> GetSurroundingTiles(const class ABoardPawn& pawn) const;

	// Updates the index of the tile the specified charater is on, and returns the new tile (it must be a walkable tile).
	const class AWalkableTile& UpdateCharacterTile(const class ABoardPawn& pawn, const FVector& position);

	// Returns the size of the map.
	FTileIndex GetSize() const;

	// A string representation of the Map.
	FString ToString() const;

private:
	// Iterates through all the tiles on the current level and generates the abstract map.
	void CreateAbstractMap();

	// Fills the map containing the characters positions.
	void FillCharactersStartingPositions();

	// Given a position, returns the index of the corresponding tile.
	FTileIndex PositionToIndex(const FVector& pos);

	// The abstract map
	TArray<TArray<const ATile*>> Map;

	// Positions of the characters
	TMap<const class ABoardPawn*, FTileIndex> CharactersPositions;

	// The positions of the bottom-left and top-right tiles, and the width and depth of each tile (they should all be the same).
	struct AbstractMapInfo {
		FVector2D Min;
		FVector2D Max;
		float TileWidth;
		float TileDepth;

		AbstractMapInfo();

		// Updates the map info with a new tile.
		void Grow(const class ATile& tile);

		// Checks whether this MapInfo has meaningful values.
		bool IsInitialized() const;

		// Returns the number of columns and rows of this AbstractMap.
		FTileIndex Size() const;
	};

	AbstractMapInfo MapInfo;
};
