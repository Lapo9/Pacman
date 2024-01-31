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

	// Called when Pacman eats a power pellet, turns all ghosts into FRIGHTENED mode (unless they are in DEAD or HOME mode).
	virtual void NotifyPowerPelletEaten() const;

	// Called when the timer of the power pellet ended, or when all ghosts got eaten.
	virtual void NotifyPowerPelletEnded() const;

	// Called when a ghost gets eaten
	virtual void NotifyGhostEaten(class AGhostPawn& ghost) const;

	// Called when Pacman dies, but it's not game over.
	virtual void NotifyPacmanDead();

	// Called when Pacman dies and there are no lives left.
	virtual void NotifyGameOver();

	// Sets all the ghosts to the specified mode. The new mode will only be applied to ghosts that are not in the modes specified in the second parameter.
	virtual void SetGhostsModeUnless(enum class EGhostMode mode, const TArray<enum class EGhostMode>& dontChange = {}) const;

	// Sets a specific ghost to the specified mode, unless the ghost is in one of the modes specified in the second parameter.
	virtual void SetGhostModeUnless(class AGhostPawn& ghost, enum class EGhostMode mode, const TArray<enum class EGhostMode>& dontChange = {}) const;

protected:
	UPROPERTY(EditAnywhere, Category = "Time mode manager")
	UTimeModeManager* TimeModeManager;
};
