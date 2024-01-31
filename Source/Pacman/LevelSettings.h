#pragma once

#include "CoreMinimal.h"
#include "GhostModeData.h"
#include "GhostPawn.h"
#include "Tile.h"
#include "WalkableTile.h"
#include "Engine/DataAsset.h"
#include "LevelSettings.generated.h"


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

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "30")) // When to activate the ghost after the last ghost activated.
	unsigned int AfterLastGhost;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "20")) // How long before the ghost come back in game after it got eaten.
	unsigned int RespawnCooldown;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1000")) // The base speed of the ghost.
	unsigned int BaseSpeed;

	UPROPERTY(EditAnywhere) // The ghost to activate.
	FString GhostId;

	AGhostPawn* Ghost; // The ghost (retrieved by his name).
};


USTRUCT()
struct FFruitScheduleItem {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "300")) // How long the fruit stays on screen
		unsigned int Duration;

	UPROPERTY(EditAnywhere) // The fruit to spawn.
		EGhostMode Fruit; //TODO 

	UPROPERTY(EditAnywhere) // Where to spawn this fruit.
		const class AWalkableTile* SpawnTile;

	UPROPERTY(EditAnywhere) // How many food items Pacman must eat before the fruit appears
		unsigned int AvailableFoodThreshold;

	// Used for sorting.
	friend bool operator<(const FFruitScheduleItem& lhs, const FFruitScheduleItem& rhs) {
		return lhs.AvailableFoodThreshold < rhs.AvailableFoodThreshold;
	}
};



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
};
