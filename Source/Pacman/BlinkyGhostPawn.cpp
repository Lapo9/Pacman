#include "BlinkyGhostPawn.h"
#include "LevelSettings.h"
#include "PacmanLevelState.h"

void ABlinkyGhostPawn::SetMode(EGhostMode mode) {
	Super::SetMode(mode);

	// Increase the speed by 5%
	if (mode == EGhostMode::CHASE) ModeSpeedMultiplier = ElroyModeSpeedMultiplier;
}


void ABlinkyGhostPawn::SetSettings(FGhostScheduleItem& settings) {
	Super::SetSettings(settings);
	if (!settings.bHasElroyMode) {
		UE_LOG(LogTemp, Warning, TEXT("Elroy settings not set for Blinky %s"), *GetName());
		ElroyModeFoodThreshold = TNumericLimits<unsigned int>::Max();
		ElroyModeSpeedMultiplier = 1.f;
	}
	else {
		ElroyModeFoodThreshold = settings.EloryModeFoodLeft;
		ElroyModeSpeedMultiplier = settings.ElroyModeSpeedMultiplier;
	}
}


UGhostModeData* ABlinkyGhostPawn::TranslateModeTagToMode(EGhostMode modeTag) const {
	if (modeTag == EGhostMode::SCATTER && Cast<APacmanLevelState>(GetWorld()->GetGameState())->GetAvailableFood() <= ElroyModeFoodThreshold) return ChaseModeSettings;
	return Super::TranslateModeTagToMode(modeTag);
}
