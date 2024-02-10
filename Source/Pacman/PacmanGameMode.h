#pragma once

#include "CoreMinimal.h"
#include "TimeModeManager.h"
#include "UiManager.h"
#include "GameFramework/GameModeBase.h"
#include "PacmanGameMode.generated.h"


// Forward declarations
class AGhostPawn;
enum class EGhostMode;


UCLASS()
class PACMAN_API APacmanGameMode : public AGameModeBase {
	GENERATED_BODY()
	
public:
	APacmanGameMode();

	// Calls BeginPlay on everything (itself included).
	virtual void StartPlay() override;

	// Initializes the game state and the mode manager.
	virtual void Init();
	
	// Starts the level (e.g. starts the timers, pawn start moving, ...)
	virtual void Start();

	// Called when Pacman gets some points by eating something.
	virtual void NotifyPointsIncreased(int totalPoints, int newPoints) const;

	// Called when Pacman eats a power pellet, turns all ghosts into FRIGHTENED mode (unless they are in DEAD or HOME mode).
	virtual void NotifyPowerPelletEaten() const;

	// Called when a ghost gets eaten
	virtual void NotifyGhostEaten(AGhostPawn& ghost) const;

	// Called when a standard food gets eaten
	virtual void NotifyAvailableFoodDecreasedBy1(unsigned int remainingFood) const;

	// Called when Pacman dies, but it's not game over.
	virtual void NotifyPacmanDead();

	// Called when Pacman dies and there are no lives left.
	virtual void NotifyGameOver();

	// Called when Pacman eats all the food.
	virtual void NotifyLevelCleared();

	// Sets all the ghosts to the specified mode. The new mode will only be applied to ghosts that are not in the modes specified in the second parameter.
	virtual void SetGhostsModeUnless(EGhostMode mode, const TArray<EGhostMode>& dontChange = {}) const;

	// Sets a specific ghost to the specified mode, unless the ghost is in one of the modes specified in the second parameter.
	virtual void SetGhostModeUnless(AGhostPawn& ghost, EGhostMode mode, const TArray<EGhostMode>& dontChange = {}) const;

	// Returns the UI manager.
	virtual UUiManager* GetUiManager();

protected:
	void LoadLevelSettings();

	UPROPERTY(EditAnywhere, Category = "Time mode manager")
	UTimeModeManager* TimeModeManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI manager")
	UUiManager* UiManager;
};
