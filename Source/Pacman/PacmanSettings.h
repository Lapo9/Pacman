#pragma once

#include "CoreMinimal.h"
#include "BoardPawn.h"
#include "GameFramework/WorldSettings.h"
#include "PacmanSettings.generated.h"

// Settings for Pacman.
UCLASS()
class PACMAN_API APacmanSettings : public AWorldSettings
{
	GENERATED_BODY()
	
public:
	APacmanSettings();

public:
	UPROPERTY(EditAnywhere) // The logical height of the floor. All Tiles and BoardPawns will be logically considered on this height, even though they may visually be placed at a different z coordinate.
	float FloorHeight;

	UPROPERTY(EditAnywhere) // The tile where each character spawns.
	TMap<ECharacterTag, const class ATile*> SpawnTiles;

	UPROPERTY(EditAnywhere) // The tile where each ghost goes to when in scatter mode.
	TMap<ECharacterTag, const class ATile*> ScatterTiles;

	UPROPERTY(EditAnywhere) // The size of the central triggers (the smaller the better, but we don't want to miss collisions).
	float PointLikeTriggersRadius;
};
