#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "WalkableTile.generated.h"

// A Tile where BoardPawns can walk on. It is responsible to catch if a BoardPawn is on it and notify this event to the pawn itself and the LevelState.
UCLASS(ClassGroup = Pacman)
class PACMAN_API AWalkableTile : public ATile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties.
	AWalkableTile();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Called when the object is placed in the editor, or its properties change, or gameplay starts.
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Returns the type of the tile.
	virtual enum class ETileType GetType() const override;

	// Should be called by the central trigger when a pawn hit it.
	virtual void PawnOnTileCenter(class ABoardPawn& pawn) const;

	// Should be called by the central trigger when a pawn leaves it.
	virtual void PawnLeftTileCenter(class ABoardPawn& pawn) const;

	// Should be called by the full trigger when a pawn left the tile.
	virtual void PawnLeftTile(class ABoardPawn& pawn) const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Triggers") // Trigger placed on the center of the tile.
	class UTileCentralTrigger* CentralTrigger; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Triggers") // Trigger to detect anithing entering the tile.
	class UTileFullTrigger* FullTrigger; 
};
