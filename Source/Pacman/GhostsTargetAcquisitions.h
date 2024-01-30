#pragma once

#include "CoreMinimal.h"
#include "PacmanUtilities.h"
#include "GhostsTargetAcquisitions.generated.h"

#define GHOST_TARGET_ACQUISITION_PARAMS const class APacmanLevelState& levelState, const class AGhostPawn& itself


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
