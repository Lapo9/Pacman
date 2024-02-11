#pragma once

#include "CoreMinimal.h"
#include "BaseFood.h"
#include "StandardFood.generated.h"


// The standard food Pacman eats.
UCLASS()
class PACMAN_API AStandardFood : public ABaseFood {
	GENERATED_BODY()
	
protected:
	void BeginPlay() override;

	// Called when another component starts overlapping with this one.
	void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) override;
};
