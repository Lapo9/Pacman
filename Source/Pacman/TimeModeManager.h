#pragma once

#include "CoreMinimal.h"
#include "GhostModeData.h"
#include "Components/ActorComponent.h"
#include "TimeModeManager.generated.h"


USTRUCT()
struct FModeScheduleItem {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "30")) // Duration of the mode in seconds.
	unsigned int Duration;

	UPROPERTY(EditAnywhere, meta = (ValidEnumValues = "STANDARD, SCATTER"))
	EGhostMode Mode;
};


USTRUCT()
struct FGhostScheduleItem {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "300")) // When to activate the ghost after the last ghost activated.
	unsigned int AfterLastGhost;

	UPROPERTY(EditAnywhere) // The ghost to activate.
	class AGhostPawn* Ghost;

	UPROPERTY(EditAnywhere) // The mode on which to set the ghost.
	EGhostMode Mode = EGhostMode::SCATTER;
};


USTRUCT()
struct FFruitScheduleItem {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "300")) // How long the fruit stays on screen
		unsigned int Duration;

	UPROPERTY(EditAnywhere) // The fruit to spawn.
		EGhostMode Fruit; //TODO 

	UPROPERTY(EditAnywhere) // How many food items Pacman must eat before the fruit appears
		unsigned int AvailableFoodThreshold;

	// Used for sorting.
	friend bool operator<(const FFruitScheduleItem& lhs, const FFruitScheduleItem& rhs) {
		return lhs.AvailableFoodThreshold < rhs.AvailableFoodThreshold;
	}
};


UCLASS()
class PACMAN_API UTimeModeManager : public UActorComponent {
	GENERATED_BODY()

public:	
	UTimeModeManager();

	// Sets all the ghosts to the current mode.
	virtual void ResumeCurrentMode();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Starts the next timer for the modes.
	virtual void StartNextModeTimer();

	// Starts the next timer for the ghosts.
	virtual void StartNextGhostTimer();

	// +1 on the food eaten in this level.
	virtual void AddFoodEaten();


	UPROPERTY(EditAnywhere, Category = "Schedule")
	TArray<FModeScheduleItem> ModesSchedule;
	FTimerHandle ModeTimer;	
	unsigned int CurrentModeIndex;

	UPROPERTY(EditAnywhere, Category = "Schedule")
	TArray<FGhostScheduleItem> GhostsSchedule;
	FTimerHandle GhostTimer;
	unsigned int CurrentGhostIndex;
	
	UPROPERTY(EditAnywhere, Category = "Schedule")
	TArray<FGhostScheduleItem> FruitsSchedule;
	unsigned int FoodEaten;
};
