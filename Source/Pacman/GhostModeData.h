#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GhostsTargetAcquisitions.h"
#include "GhostsOnCollideReactions.h"
#include "GhostModeData.generated.h"


UENUM()
enum class EGhostMode {
	HOME, SCATTER, STANDARD, FRIGHTENED, DEAD, ANGRY
};


UCLASS()
class PACMAN_API UGhostModeData : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EGhostMode Mode;

	UPROPERTY(EditAnywhere)
	float StandardSpeed;

	UPROPERTY(EditAnywhere)
	float TunnelSpeed;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGhostTargetAcquisitionBase> TargetAcquisitionClass;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGhostOnCollideReactionBase> OnCollideReactionClass;
};
