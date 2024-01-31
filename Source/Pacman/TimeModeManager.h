#pragma once

#include "CoreMinimal.h"
#include "GhostModeData.h"
#include "LevelSettings.h"
#include "Components/ActorComponent.h"
#include "TimeModeManager.generated.h"


UCLASS()
class PACMAN_API UTimeModeManager : public UActorComponent {
	GENERATED_BODY()

public:	
	UTimeModeManager();

	// Returns the current mode.
	virtual EGhostMode GetCurrentMode();

	// Binds the ghost, fruit and modes schedule to this component.
	virtual void SetSettings(ULevelSettings* settings);

	// Starts the level.
	virtual void Start();

	// Should be called when a ghost dies.
	virtual void NotifyGhostDied(AGhostPawn& ghost);

	// +1 on the food eaten in this level. If a fruit threshold is reached, the fruit is spawned.
	virtual void NotifyFoodEaten();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Starts the next timer for the modes.
	virtual void StartNextModeTimer();

	// Starts the next timer for the ghosts.
	virtual void StartNextGhostTimer();

	// Activates the specified ghost in the current mode.
	virtual void ActivateGhost(AGhostPawn& ghost);
	
	UPROPERTY(VisibleAnywhere)
	ULevelSettings* CurrentLevelSettings;

	FTimerHandle ModeTimer;	
	unsigned int CurrentModeIndex;

	FTimerHandle GhostTimer;
	unsigned int CurrentGhostIndex;
	
	unsigned int FoodEaten;

	TMap<AGhostPawn*, FTimerHandle> GhostRespawnTimers;

	bool Started;
};
