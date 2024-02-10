#pragma once

#include "CoreMinimal.h"
#include "PacmanUtilities.h"
#include "GameFramework/Actor.h"
#include "BaseFood.generated.h"


// Forward declarations
class APacmanLevelState;
class UAudioComponent;
class USoundBase;


// The base abstract class for food items.
// A food item has the responsibility to detect when he is eaten by Pacman, and notify this to the PacmanLevelState.
UCLASS(Abstract)
class PACMAN_API ABaseFood : public AActor {
	GENERATED_BODY()

public:
	ABaseFood();

	// Destroys this actor (with an animation/particle effect)
	virtual void AutoDestroy();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()	// Called when another component starts overlapping with this one.
	virtual void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) PURE_VIRTUAL(ABaseFood::OnBeginOverlap, );

	UPROPERTY(EditAnywhere, Category = "Pacman|Point system") // How many points eating this food gives.
	unsigned int Value;

	UPROPERTY(VisibleAnywhere) // Component to place this actor on the scene.
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere) // Mesh
	UStaticMeshComponent* Mesh;

	// Reference to the level state to notify when food is eaten
	APacmanLevelState* LevelState;
};
