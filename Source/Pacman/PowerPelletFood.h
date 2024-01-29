#pragma once

#include "CoreMinimal.h"
#include "BaseFood.h"
#include "PowerPelletFood.generated.h"


// Power pellets that turn ghosts into frightened mode when eaten.
UCLASS()
class PACMAN_API APowerPelletFood : public ABaseFood
{
	GENERATED_BODY()
public:
	APowerPelletFood();

protected:
	void BeginPlay() override;

	// Destroys this actor (with an animation/particle effect)
	void AutoDestroy();

	// Called when another component starts overlapping with this one.
	void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) override;

};
