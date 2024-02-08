#pragma once

#include "CoreMinimal.h"
#include "PacmanUtilities.h"
#include "GhostsTargetAcquisitions.generated.h"

// Forward declarations
class APacmanLevelState;
class AGhostPawn;

#define GHOST_TARGET_ACQUISITION_PARAMS const APacmanLevelState& levelState, const AGhostPawn& itself


UCLASS(Abstract)
class PACMAN_API UGhostTargetAcquisitionBase : public UObject {
	GENERATED_BODY()
public:
	virtual FTileIndex AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const PURE_VIRTUAL(UGhostTargetAcquisitionBase::AcquireTarget, return FTileIndex{};);
};


UCLASS()
class PACMAN_API UGhostTargetAcquisitionBlinky final : public UGhostTargetAcquisitionBase {
	GENERATED_BODY()
public:
	FTileIndex AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const;
};


UCLASS()
class PACMAN_API UGhostTargetAcquisitionPinky final : public UGhostTargetAcquisitionBase {
	GENERATED_BODY()
public:
	FTileIndex AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const;
};


UCLASS()
class PACMAN_API UGhostTargetAcquisitionInky final : public UGhostTargetAcquisitionBase {
	GENERATED_BODY()
public:
	FTileIndex AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const;
};


UCLASS()
class PACMAN_API UGhostTargetAcquisitionClyde final : public UGhostTargetAcquisitionBase {
	GENERATED_BODY()
public:
	FTileIndex AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const;
};


UCLASS()
class PACMAN_API UGhostTargetAcquisitionScatter final : public UGhostTargetAcquisitionBase {
	GENERATED_BODY()
public:
	FTileIndex AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const;
};


UCLASS()
class PACMAN_API UGhostTargetAcquisitionRandom final : public UGhostTargetAcquisitionBase {
	GENERATED_BODY()
public:
	FTileIndex AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const;
};


UCLASS()
class PACMAN_API UGhostTargetAcquisitionDead final : public UGhostTargetAcquisitionBase {
	GENERATED_BODY()
public:
	FTileIndex AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const;
};


UCLASS()
class PACMAN_API UGhostTargetAcquisitionHome final : public UGhostTargetAcquisitionBase {
	GENERATED_BODY()
public:
	FTileIndex AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const;
};



namespace TargetAcquisitionFunctions {
	FTileIndex Blinky(GHOST_TARGET_ACQUISITION_PARAMS);
	FTileIndex Pinky(GHOST_TARGET_ACQUISITION_PARAMS);
	FTileIndex Inky(GHOST_TARGET_ACQUISITION_PARAMS);
	FTileIndex Clyde(GHOST_TARGET_ACQUISITION_PARAMS);
	FTileIndex Home(GHOST_TARGET_ACQUISITION_PARAMS);
	FTileIndex Scatter(GHOST_TARGET_ACQUISITION_PARAMS);
	FTileIndex Dead(GHOST_TARGET_ACQUISITION_PARAMS);
	FTileIndex Random(GHOST_TARGET_ACQUISITION_PARAMS);
}
