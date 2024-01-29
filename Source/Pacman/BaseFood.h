#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseFood.generated.h"


// The base abstract class for food items
UCLASS(Abstract)
class PACMAN_API ABaseFood : public AActor {
	GENERATED_BODY()

public:
	ABaseFood();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()	// Called when another component starts overlapping with this one.
	virtual void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) PURE_VIRTUAL(ABaseFood::OnBeginOverlap, );

	UPROPERTY(EditAnywhere, Category = "Point system") // How many points eating this food gives.
	unsigned int Value;

	UPROPERTY(EditAnywhere) // Component to place this actor on the scene.
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere) // Mesh
	UStaticMeshComponent* Mesh;

	// Reference to the level state to notify when food is eaten
	class APacmanLevelState* LevelState;
};
