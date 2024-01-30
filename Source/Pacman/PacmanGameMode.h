#pragma once

#include "CoreMinimal.h"
#include "TimeModeManager.h"
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

	// Called when a ghost gets eaten
	virtual void NotifyGhostEaten(class AGhostPawn& ghost) const;

	// Sets all the ghosts to the specified mode.
	virtual void SetGhostsMode(enum class EGhostMode mode) const;

	// Sets a specific ghost to the specified mode.
	virtual void SetGhostMode(class AGhostPawn& ghost, enum class EGhostMode mode) const;

protected:
	UPROPERTY(EditAnywhere, Category = "Time mode manager")
	UTimeModeManager* TimeModeManager;
};
