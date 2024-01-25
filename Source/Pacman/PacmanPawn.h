#pragma once

#include "CoreMinimal.h"
#include "BoardPawn.h"
#include "PacmanPawn.generated.h"


// The Pacman on the maze.
UCLASS()
class PACMAN_API APacmanPawn : public ABoardPawn
{
	GENERATED_BODY()
	
public:
	APacmanPawn();

	// Called to notify the pawn that it is at the center of the current tile
	virtual void OnTileCenter(const class ATile& tile);

	// Input actions

	// Pacman turns into the specified direction.
	void TurnDirection(enum class EMovingDirection dir);

protected:
	void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Real-time info") // The direction Pacman wants to move.
	EMovingDirection InputDirection;
};