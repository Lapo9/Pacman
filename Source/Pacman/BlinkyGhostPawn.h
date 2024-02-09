#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "BlinkyGhostPawn.generated.h"

// Blinky has a slightly different behavior than other ghosts, because, when the remaining food is low, its speed increases and his scatter mode is never activated.
UCLASS()
class PACMAN_API ABlinkyGhostPawn : public AGhostPawn
{
	GENERATED_BODY()

	virtual void SetMode(EGhostMode mode) override;

	virtual void SetSettings(struct FGhostScheduleItem& settings) override;
	
protected:
	virtual UGhostModeData* TranslateModeTagToMode(EGhostMode modeTag) const override;

	UPROPERTY(VisibleAnywhere, Category = "Pacman|Elroy") // How much food must remain to activate cruise elroy mode.
	unsigned int ElroyModeFoodThreshold;

	UPROPERTY(VisibleAnywhere, Category = "Pacman|Elroy") // Speed multiplier when in elroy mode
	float ElroyModeSpeedMultiplier;
};
