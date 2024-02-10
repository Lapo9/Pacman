#pragma once

#include "CoreMinimal.h"
#include "GhostModeData.h"
#include "GhostPawn.h"
#include "Tile.h"
#include "FruitFood.h"
#include "WalkableTile.h"
#include "Materials/MaterialParameterCollection.h"
#include "Engine/DataAsset.h"
#include "LevelSettings.generated.h"


// One item of the modes schedule.
USTRUCT()
struct FModeScheduleItem {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "30")) // Duration of the mode in seconds.
		unsigned int Duration;

	UPROPERTY(EditAnywhere, meta = (ValidEnumValues = "CHASE, SCATTER"))
	EGhostMode Mode;
};


// One item of the ghosts activations schedule and info about the ghost parameter to apply in this level (i.e. speed and respawn cooldown).
USTRUCT()
struct FGhostScheduleItem {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "30")) // When to activate the ghost after the last ghost activated.
	unsigned int AfterLastGhost;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "20")) // How long before the ghost come back in game after it got eaten.
	unsigned int RespawnCooldown;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1000")) // The base speed of the ghost.
	unsigned int BaseSpeed;

	UPROPERTY(EditAnywhere)
	bool bHasElroyMode;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bHasElroyMode", EditConditionHides, ClampMin = "0"))
	unsigned int EloryModeFoodLeft;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bHasElroyMode", EditConditionHides, ClampMin = "0", ClampMax = "2"))
	float ElroyModeSpeedMultiplier;

	UPROPERTY(EditAnywhere) // The ghost to activate.
	FString GhostId;

	AGhostPawn* Ghost; // The ghost (retrieved by his name).
};


// One item of the fruit spawn schedule.
USTRUCT()
struct FFruitScheduleItem {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere) // The fruit to spawn.
	TSubclassOf<AFruitFood> Fruit;

	UPROPERTY(EditAnywhere) // Where to spawn this fruit.
	FVector SpawnPosition;

	UPROPERTY(EditAnywhere) // How many food items Pacman must eat before the fruit appears
	unsigned int RemainingFoodThreshold;

	// Used for sorting.
	friend bool operator<(const FFruitScheduleItem& lhs, const FFruitScheduleItem& rhs) {
		return lhs.RemainingFoodThreshold < rhs.RemainingFoodThreshold;
	}
};



// Data object that holds the modes, ghost activation and fruit spawning schedules for a level.
// This object can be bound to a TimeModeManager, and the TimeModeManager will execute these schedules.
UCLASS()
class PACMAN_API ULevelSettings : public UDataAsset {
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Schedule")
	TArray<FModeScheduleItem> ModesSchedule;

	UPROPERTY(EditAnywhere, Category = "Schedule")
	TArray<FGhostScheduleItem> GhostsSchedule;

	UPROPERTY(EditAnywhere, Category = "Schedule")
	TArray<FFruitScheduleItem> FruitsSchedule;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1000"))
	unsigned int PacmanBaseSpeed;

	UPROPERTY(EditAnywhere, Category = "Power pellet", meta = (ClampMin = "0", ClampMax = "20.0"))
	float PowerPelletDuration;

	// When to show the visual hint that the power pellet is ending. If the value is bigger than Power Pellet Duration, the hint will be showed right away.
	UPROPERTY(EditAnywhere, Category = "Power pellet", meta = (ClampMin = "0", ClampMax = "20.0"))
	float PowerPelletEndingTimeRemaining;

	// Object used to globally change some parameters of the material of the FRIGHTENED ghosts, in order to provide a cisual hint for when the ghost is ending its FRIGHTENED mode.
	UPROPERTY(EditAnywhere, Category = "Power pellet")
	UMaterialParameterCollection* FrightenedModeMaterialVisualHintParams;
};
