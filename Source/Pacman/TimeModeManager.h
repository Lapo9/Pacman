#pragma once

#include "CoreMinimal.h"
#include "GhostModeData.h"
#include "LevelSettings.h"
#include "Components/ActorComponent.h"
#include "TimeModeManager.generated.h"


// Forward declarations
class APacmanGameMode;


// Class responsible to change modes, activate ghosts and spawn fruits based on the time passed in the level.
// Before using this object it is important to bind a LevelSettings object via Init.
UCLASS()
class PACMAN_API UTimeModeManager : public UActorComponent {
	GENERATED_BODY()

public:	
	UTimeModeManager();

	// Returns the current mode.
	virtual EGhostMode GetCurrentMode();

	// Resets the component and loads the specified settings to the ghosts, Pacman and initializes the schedules. Doesn't start the timers.
	virtual void Init(ULevelSettings* settings);

	// Starts the level.
	virtual void Start();


	// Should be called when a ghost dies.
	virtual void NotifyGhostDied(AGhostPawn& ghost);

	//  If a fruit threshold is reached, the fruit is spawned.
	virtual void NotifyStandardFoodDecreasedBy1(unsigned int remainingFood);

	// Should be called when Pacman eats a power pellet, turns all ghosts into FRIGHTENED mode (unless they are in DEAD or HOME mode).
	virtual void NotifyPowerPelletEaten();

	// Should be called when a power pellet ends (due to time or becasue all ghosts got eaten).
	virtual void NotifyPowerPelletEnded();

	// Should be called when the level ends, for whatever reason.
	virtual void NotifyLevelEnded();

	// Activates the specified ghost in the current mode.
	virtual void ActivateGhost(AGhostPawn& ghost);

protected:
	// Resets all the timers and counters in the level.
	virtual void Reset();

	// Starts the next timer for the modes.
	virtual void StartNextModeTimer();

	// Starts the next timer for the ghosts.
	virtual void StartNextGhostTimer();
	
	UPROPERTY(VisibleInstanceOnly, Category = "Pacman|Real-time info")
	ULevelSettings* CurrentLevelSettings;

	FTimerHandle ModeTimer;	
	unsigned int CurrentModeIndex;

	FTimerHandle GhostTimer;
	unsigned int CurrentGhostIndex;

	TMap<AGhostPawn*, FTimerHandle> GhostRespawnTimers;

	FTimerHandle PowerPelletTimer;

	bool bStarted; // Has the level already started?

	APacmanGameMode* GameMode; // The game mode that owns this component
};
