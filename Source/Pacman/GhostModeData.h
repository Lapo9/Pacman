#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GhostsTargetAcquisitions.h"
#include "GhostsOnCollideReactions.h"
#include "GhostModeData.generated.h"


// Tag identifying the mode a GhostPawn can be set to.
UENUM()
enum class EGhostMode {
	HOME UMETA(DisplayName = "HOME"), // Target your home tile
	SCATTER UMETA(DisplayName = "SCATTER"), // Target your scatter tile
	CHASE UMETA(DisplayName = "CHASE"), // Chase Pacman, based on your algorithm
	FRIGHTENED UMETA(DisplayName = "FRIGHTENED"), // Target a random tile at every turn, can be eaten by Pacman
	DEAD UMETA(DisplayName = "DEAD") // Target your home tile, once there go in HOME mode; cannot kill nor be killed by Pacman
};


// Data object holding the parameters and behaviours of each one of the modes a GhostPawn can be set to.
// Each GhostPawn will have one of this data assets for each mode he can be set to, and when the mode is activated, it will retrieve its new parameters in this object.
// Examples of parameters can be the speed of the ghost, or the algorithm controlling how it chooses the new target tile during decision making.
UCLASS()
class PACMAN_API UGhostModeData : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EGhostMode Mode;

	UPROPERTY(EditAnywhere)
	float SpeedMultiplier;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGhostTargetAcquisitionBase> TargetAcquisitionClass;

	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (EditCondition = "Mode != EGhostMode::CHASE", EditConditionHides))
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (EditCondition = "Mode != EGhostMode::CHASE", EditConditionHides))
	UMaterial* Material;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGhostOnCollideReactionBase> OnCollideReactionClass;
};
