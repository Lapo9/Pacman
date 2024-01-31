#include "BlinkyGhostPawn.h"

void ABlinkyGhostPawn::SetMode(EGhostMode mode) {
	Super::SetMode(mode);

	// Increase the speed by 5%
	if (mode == EGhostMode::STANDARD) ModeSpeedMultiplier = 1.05;
}


UGhostModeData* ABlinkyGhostPawn::TranslateModeTagToMode(EGhostMode modeTag) const {
	if (modeTag == EGhostMode::SCATTER && false /*TODO There is low food left*/) return StandardMode;
	return Super::TranslateModeTagToMode(modeTag);
}
