#pragma once

#include "CoreMinimal.h"
#include "BoardPawn.h"
#include "GhostPawn.generated.h"

UCLASS()
class PACMAN_API AGhostPawn : public ABoardPawn
{
	GENERATED_BODY()

public:
	AGhostPawn();

	virtual void BeginPlay() override;

	// Called to notify the pawn that it is at the center of the current tile.
	virtual void OnTileCenter(const class ATile& tile) override;

	// Input actions

	// The pawn turns into the specified direction.
	void TurnDirection(enum class EMovingDirection dir);

protected:
	class AGhostAiController* AiController;
};
