#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "GhostBlinky.generated.h"

// Blinky is the red ghost, its target is directly Pacman.
UCLASS()
class PACMAN_API AGhostBlinky : public AGhostPawn {
	GENERATED_BODY()

public:
	AGhostBlinky();

	void BeginPlay() override;
};
