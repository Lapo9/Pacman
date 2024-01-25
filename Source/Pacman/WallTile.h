#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "WallTile.generated.h"

// A Tile not accessible by BoardPawns.
UCLASS(ClassGroup = Pacman)
class PACMAN_API AWallTile : public ATile
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWallTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Returns the type of the tile.
	virtual enum class ETileType GetType() const override;
};
