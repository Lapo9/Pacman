#pragma once

#include "CoreMinimal.h"
#include "WalkableTile.h"
#include "TeleportTile.generated.h"


// A tile which teleports a board pawn interacting with it to a target tile.
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
