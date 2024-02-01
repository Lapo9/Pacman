#include "BlinkyGhostPawn.h"
#include "PacmanLevelState.h"

void ABlinkyGhostPawn::SetMode(EGhostMode mode) {
	Super::SetMode(mode);

	// Increase the speed by 5%
	if (mode == EGhostMode::CHASE) ModeSpeedMultiplier = 1.05;
}


UGhostModeData* ABlinkyGhostPawn::TranslateModeTagToMode(EGhostMode modeTag) const {
	if (modeTag == EGhostMode::SCATTER && Cast<APacmanLevelState>(GetWorld()->GetGameState())->GetAvailableFood() <= CruiseElroyFoodThreshold) return ChaseModeSettings;
	return Super::TranslateModeTagToMode(modeTag);
}
