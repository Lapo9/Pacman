#pragma once

#include "CoreMinimal.h"
#include "TimeModeManager.h"
#include "GameFramework/WorldSettings.h"
#include "PacmanSettings.generated.h"

// Stores the global settings of Pacman (the ones that are not level related).
UCLASS()
class PACMAN_API APacmanSettings : public AWorldSettings
{
	GENERATED_BODY()
	
public:
	APacmanSettings();

public:
	UPROPERTY(EditAnywhere) // The logical height of the floor. All Tiles and BoardPawns will be logically considered on this height, even though they may visually be placed at a different z coordinate.
	float FloorHeight;

	UPROPERTY(EditAnywhere) // The size of the central triggers (the smaller the better, but we don't want to miss collisions).
	float PointLikeTriggersRadius;

	UPROPERTY(EditAnywhere) // Score to reach to get an extra life.
	TArray<unsigned int> PointsForExtraLives;

	UPROPERTY(EditAnywhere) // List of data assets containing level-related settings.
	TArray<ULevelSettings*> LevelsSettings;
};
