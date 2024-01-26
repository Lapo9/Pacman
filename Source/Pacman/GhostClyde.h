#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "GhostClyde.generated.h"

// Clyde is the orange ghost, its target depends on Pacman, but if he is too close to Pacman, he will change target.
UCLASS()
class PACMAN_API AGhostClyde : public AGhostPawn {
	GENERATED_BODY()

public:
	AGhostClyde();

	void BeginPlay() override;
};
