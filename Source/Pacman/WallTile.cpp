#include "WallTile.h"
#include "AbstractMap.h"

// Sets default values
AWallTile::AWallTile() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void AWallTile::BeginPlay() {
	Super::BeginPlay();
}


// Returns the type of the tile.
ETileType AWallTile::GetType() const {
	return ETileType::WALL;
}
