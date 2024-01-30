#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PacmanGameMode.generated.h"


UCLASS()
class PACMAN_API APacmanGameMode : public AGameModeBase {
	GENERATED_BODY()
	
public:
	APacmanGameMode();

	// Called before BeginPlay on each component.
	virtual void StartPlay() override;

	// Called when Pacman eats a power pellet, turns all ghosts into frightened mode.
	virtual void NotifyPowerPelletEaten() const;

	// Called when the timer of the power pellet ended, or when all ghosts got eaten.
	virtual void NotifyPowerPelletEnded() const;

	// Called when it is time to change mode because of time
	virtual void NotifyChangeOfModeDueToTime() const;

	// Called when a ghost gets eaten
	virtual void NotifyGhostEaten(class AGhostPawn& ghost) const;

private:
	virtual void SetGhostsMode(enum class EGhostMode mode) const;
};
