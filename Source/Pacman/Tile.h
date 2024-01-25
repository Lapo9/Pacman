#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbstractMap.h"
#include "Tile.generated.h"

// One cell of the map.
UCLASS(Abstract)
class PACMAN_API ATile : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties.
	ATile();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

public:	

	// Returns the coordinates of the logical center of the tile.
	FVector GetCenter() const;

	// Returns the axis aligned extents of this tile.
	virtual FVector GetExtents() const;

	// Returns the type of the tile.
	virtual enum class ETileType GetType() const PURE_VIRTUAL(ATile::StaticClass(), return ETileType::WALL;);

protected:
	UPROPERTY() // Component to place this actor on the scene.
	class USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere) // The mesh.
	class UStaticMeshComponent* Mesh;

public:
	TPair<int, int> Index;
};
