#include "TileFullTrigger.h"
#include "BoardPawn.h"
#include "WalkableTile.h"
#include "PacmanLevelState.h"
#include "Components/SphereComponent.h"


UTileFullTrigger::UTileFullTrigger() {
	OnComponentEndOverlap.AddDynamic(this, &UTileFullTrigger::OnEndOverlap);

	// Set up collision channels responses
	SetCollisionObjectType(ObjectChannel_World2d); // World2d object channel
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Disable all collisions
	SetCollisionResponseToChannel(ObjectChannel_World2d, ECollisionResponse::ECR_Overlap); // enable World2d object channel
}


// Sets the location of the component based on the tile position, and the radius based on the bounding box of the mesh attached to the tile.
void UTileFullTrigger::Init(const AWalkableTile& tile) {
	Tile = &tile;

	SetWorldLocation(tile.GetCenter());

	auto extents = tile.GetExtents();
	SetSphereRadius(fminf(extents.X, extents.Y)); // Set the radius to the smaller extent of the bounding box (between x and y)
}


// Called when another component ends overlapping with this one.
void UTileFullTrigger::OnEndOverlap(class UPrimitiveComponent* overlappedComponent, class AActor* otherActor, class UPrimitiveComponent* otherComponent, int32 otherBodyIndex) {
	if (auto otherBoardPawn = Cast<ABoardPawn>(otherActor); otherBoardPawn) {
		Tile->PawnLeftTile(*otherBoardPawn);
	}
}
