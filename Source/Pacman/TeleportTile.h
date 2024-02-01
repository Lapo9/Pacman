#pragma once

#include "CoreMinimal.h"
#include "WalkableTile.h"
#include "TeleportTile.generated.h"


// A TeleportTile is a WalkableTile that teleports any BoardPawn interacting with its center to a target tile.
// In order to avoid possible infinite teleport loops (e.g. A -> B -> C -> ... -> A -> ...), when a BoardPawn is teleported to a new WalkableTile, it won't teleport the BoardPawn again until it leaves and re-enters the center.
UCLASS()
class PACMAN_API ATeleportTile : public AWalkableTile {
	GENERATED_BODY()
	
public:
	ATeleportTile();

	// Should be called by the central trigger when a pawn hit it.
	virtual void PawnOnTileCenter(class ABoardPawn& pawn) const override;

	// Should be called by the central trigger when a pawn leaves it.
	virtual void PawnLeftTileCenter(class ABoardPawn& pawn) const override;

protected:
	UPROPERTY(EditInstanceOnly, Category = "Pacman|Movement") // Where the teleport ends.
	AWalkableTile* TeleportToTile;
};
