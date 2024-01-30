#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "BaseFood.h"
#include "WalkableTile.generated.h"


// A Tile where BoardPawns can walk on. It is responsible to catch if a BoardPawn is on it and notify this event to the pawn itself and the LevelState.
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

	// Called when the object is placed in the editor, or its properties change, or gameplay starts.
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Should be called by the central trigger when a pawn hit it.
	virtual void PawnOnTileCenter(class ABoardPawn& pawn) const;

	// Should be called by the central trigger when a pawn leaves it.
	virtual void PawnLeftTileCenter(class ABoardPawn& pawn) const;

	// Should be called by the full trigger when a pawn left the tile.
	virtual void PawnLeftTile(class ABoardPawn& pawn) const;
	
protected:
	// Spawns the food item (if required).
	void SpawnFood() const;

	UPROPERTY(VisibleAnywhere, Category = "Triggers") // Trigger placed on the center of the tile.
	class UTileCentralTrigger* CentralTrigger; 

	UPROPERTY(VisibleAnywhere, Category = "Triggers") // Trigger to detect anithing entering the tile.
	class UTileFullTrigger* FullTrigger; 

	UPROPERTY(EditAnywhere, Category = "Point system") // Which point this tile should spawn.
	TSubclassOf<ABaseFood> FoodToSpawn;

	UPROPERTY(EditAnywhere, Category = "Point system") // How much the food should be spawned over the tile.
	float DistanceFromTile;
};
