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
	
protected:
	virtual UGhostModeData* TranslateModeTagToMode(EGhostMode modeTag) const override;

	UPROPERTY(EditAnywhere, Category = "Pacman|Movement", meta = (ClampMin = "0", ClampMax = "500")) // How much food must remain to activate cruise elroy mode.
	unsigned int CruiseElroyFoodThreshold;
};
