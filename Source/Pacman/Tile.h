#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbstractMap.h"
#include "PacmanUtilities.h"
#include "Tile.generated.h"

// One cell of the map.
UCLASS(Abstract)
class PACMAN_API ATile : public AActor {
	GENERATED_BODY()
	
public:	
	ATile();
	ATile(ETileType type);

protected:
	virtual void BeginPlay() override;

public:	

	// Returns the coordinates of the logical center of the tile.
	FVector GetCenter() const;

	// Returns the coordinates of the center of the tile (only X and Y).
	FVector2D GetLocation2d() const;

	// Returns the axis aligned extents of this tile.
	virtual FVector GetExtents() const;

	// Returns the type of the tile.
	enum class ETileType GetType() const;

protected:
	UPROPERTY(VisibleAnywhere) // Component to place this actor on the scene.
	class USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere) // The mesh.
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Pacman", meta = (ValidEnumValues = "WALKABLE, TUNNEL", EditCondition = "TileType == ETileType::WALKABLE || TileType == ETileType::TUNNEL"))
	ETileType TileType;

public:
	UPROPERTY(VisibleAnywhere, Category = "Pacman")
	FTileIndex Index;
};
