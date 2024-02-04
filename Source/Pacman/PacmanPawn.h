#pragma once

#include "CoreMinimal.h"
#include "BoardPawn.h"
#include "PacmanPawn.generated.h"


// Forward declarations
class AWalkableTile;
enum class EMovingDirection;


// The Pacman on the maze, subclass of BoardPawn.
// Pacman receives the input direction by a human player virtually at every frame, but it will try to change direction only in 3 cases:
// 1) When he receives the OnTileCenter event from a WalkableTile;
// 2) When the new direction is the opposite of the current one;
// 3) If Pacman is standing still (speed = 0) on a Tile.
UCLASS()
class PACMAN_API APacmanPawn : public ABoardPawn
{
	GENERATED_BODY()
	
public:
	APacmanPawn();

	virtual void Init() override;

	// Called to notify the pawn that it is at the center of the current tile
	virtual void OnTileCenter(const AWalkableTile& tile) override;

	// Pacman turns into the specified direction.
	void TurnDirection(EMovingDirection dir);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pacman|Real-time info") // The direction Pacman wants to move.
	EMovingDirection InputDirection;
};
