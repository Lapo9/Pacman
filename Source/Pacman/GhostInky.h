#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "GhostInky.generated.h"

// Inky is the blue ghost, its target is dependant on both Pacman and Blinky.
UCLASS()
class PACMAN_API AGhostInky final : public AGhostPawn {
	GENERATED_BODY()
	
public:
	AGhostInky();

	void BeginPlay() override;
};
