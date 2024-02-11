#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "BaseFood.h"
#include "WalkableTile.generated.h"


// Forward declarations
class ABoardPawn;
class UTileCentralTrigger;
class UTileFullTrigger;


// A Tile where BoardPawns can walk on. 
// It is responsible to detect 2 events:
// 1) When a BoardPawn leaves the WalkableTile, it notifies this event to the AbstractMap and to the BoardPawn itself.
// 2) When a BoardPawn entrs the center of the WalkableTile, it notifies this event to the BoardPawn itself.
// The collider to represent the center of the WalkableTile is a sphere, whose radius is governed by the PointLikeTriggersRadius variable in PacmanSettings.
// WalkableTiles can be placed at any height on the Z coordinate, but their "logical height" (the height of the colliders) is determined by the FloorHeight variable in PacmanSettings.
UCLASS(ClassGroup = Pacman)
class PACMAN_API AWalkableTile : public ATile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties.
	AWalkableTile();
	AWalkableTile(ETileType type);

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

public:
	// Spawns the food (if this tile has a food item to spawn on itself)
	virtual void Init() override;

	// Should be called by the central trigger when a pawn hit it.
	virtual void PawnOnTileCenter(ABoardPawn& pawn) const;

	// Should be called by the central trigger when a pawn leaves it.
	virtual void PawnLeftTileCenter(ABoardPawn& pawn) const;

	// Should be called by the full trigger when a pawn left the tile.
	virtual void PawnLeftTile(ABoardPawn& pawn) const;

	// Returns the value to increase or decrease the speed of a BoardPawn on this tile.
	float GetSpeedMultiplier() const;
	
protected:
	// Spawns the food item (if required).
	void SpawnFood() const;

	UPROPERTY(VisibleAnywhere, Category = "Pacman|Triggers") // Trigger placed on the center of the tile.
	UTileCentralTrigger* CentralTrigger; 

	UPROPERTY(VisibleAnywhere, Category = "Pacman|Triggers") // Trigger to detect anithing entering the tile.
	UTileFullTrigger* FullTrigger; 

	UPROPERTY(EditAnywhere, Category = "Pacman|Point system") // Which point this tile should spawn.
	TSubclassOf<ABaseFood> FoodToSpawn;
	mutable ABaseFood* SpawnedFood;

	UPROPERTY(EditAnywhere, Category = "Pacman|Point system") // How much the food should be spawned over the tile.
	float DistanceFromTile;

	UPROPERTY(EditAnywhere, Category = "Pacman|Movement", meta = (ClampMin = "0", ClampMax = "3")) // Increases or decreases the speed of the board pawns on this tile
	float SpeedMultiplier;
};
