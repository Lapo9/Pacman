#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "GhostPinky.generated.h"

// Pinky is the pink ghost, its target is 4 tiles ahead of Pacman.
UCLASS()
class PACMAN_API AGhostPinky : public AGhostPawn {
	GENERATED_BODY()

public:
	AGhostPinky();

	void BeginPlay() override;
};
