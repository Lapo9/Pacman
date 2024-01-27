#pragma once

#include "CoreMinimal.h"
#include "BaseFood.h"
#include "StandardFood.generated.h"


UCLASS()
class PACMAN_API AStandardFood : public ABaseFood {
	GENERATED_BODY()
	
public:
	AStandardFood();

protected:
	void BeginPlay() override;

	// Destroys this actor (with an animation/particle effect)
	void AutoDestroy();

	// Called when another component starts overlapping with this one.
	void OnBeginOverlap(class AActor* overlappedActor, class AActor* otherActor) override;
};
