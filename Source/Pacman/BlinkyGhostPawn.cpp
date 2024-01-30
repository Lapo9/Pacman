#include "BlinkyGhostPawn.h"

void ABlinkyGhostPawn::SetMode(EGhostMode mode) {
	Super::SetMode(mode);

	// Increase speed by 5%
	StandardSpeed *= 1.05;
	TunnelSpeed *= 1.05;
}


UGhostModeData* ABlinkyGhostPawn::TranslateModeTagToMode(EGhostMode modeTag) const {
	if (modeTag == EGhostMode::SCATTER && false /*TODO There is low food left*/) return StandardMode;
	return Super::TranslateModeTagToMode(modeTag);
}
