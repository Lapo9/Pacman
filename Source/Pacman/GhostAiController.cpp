#include "GhostAiController.h"
#include "BoardPawn.h"
#include "PacmanLevelState.h"
#include "GhostPawn.h"

void AGhostAiController::Init(TFunction<const class ATile* (const class UAbstractMap&)> acquireNewTarget) {
	AcquireNewTarget = acquireNewTarget;
}

void AGhostAiController::GhostOnTileCenter(const ATile& tile) {
	Target = AcquireNewTarget(Cast<APacmanLevelState>(GetWorld()->GetGameState())->GetAbstractMap());
	auto newDirection = ComputeBestDirection();
	Cast<AGhostPawn>(GetPawn())->TurnDirection(newDirection);
}

EMovingDirection AGhostAiController::ComputeBestDirection() const {
	return EMovingDirection();
}
