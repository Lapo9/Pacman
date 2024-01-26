#pragma once

#include "CoreMinimal.h"
#include "PacmanUtilities.h"

// Target acquisition functions usable for GhostAiControllers.
class PACMAN_API GhostsTargetAcquisitions {
public:
	static FTileIndex Blinky(const class UAbstractMap& map, const class APacmanPawn& pacman, const class ABoardPawn& itself, const TMap<enum class ECharacterTag, class ABoardPawn*>& otherPawnss);
	static FTileIndex Pinky(const class UAbstractMap& map, const class APacmanPawn& pacman, const class ABoardPawn& itself, const TMap<enum class ECharacterTag, class ABoardPawn*>& otherPawns);
	static FTileIndex Inky(const class UAbstractMap& map, const class APacmanPawn& pacman, const class ABoardPawn& itself, const TMap<enum class ECharacterTag, class ABoardPawn*>& otherPawns);
	static FTileIndex Clyde(const class UAbstractMap& map, const class APacmanPawn& pacman, const class ABoardPawn& itself, const TMap<enum class ECharacterTag, class ABoardPawn*>& otherPawns);
};
