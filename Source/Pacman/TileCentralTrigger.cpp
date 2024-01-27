#include "TileCentralTrigger.h"
#include "BoardPawn.h"
#include "WalkableTile.h"
#include "PacmanSettings.h"
#include "Components/SphereComponent.h"
#include "Engine/EngineTypes.h"


UTileCentralTrigger::UTileCentralTrigger() {
	OnComponentBeginOverlap.AddDynamic(this, &UTileCentralTrigger::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UTileCentralTrigger::OnEndOverlap);

	// Set up collision channels responses
	SetCollisionObjectType(ObjectChannel_World2d); // World2d object channel
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Disable all collisions
	SetCollisionResponseToChannel(ObjectChannel_World2d, ECollisionResponse::ECR_Overlap); // enable World2d object channel
}


// Sets the location of the component based on the tile position, and the radius of the sphere trigger.
void UTileCentralTrigger::Init(const AWalkableTile& tile) {
	Tile = &tile;
	
	SetWorldLocation(Tile->GetCenter());
	SetSphereRadius(Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->PointLikeTriggersRadius);
}


// Called when another component starts overlapping with this one.
void UTileCentralTrigger::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) {
	if (auto otherBoardPawn = Cast<ABoardPawn>(otherActor); otherBoardPawn) {
		Tile->PawnOnTileCenter(*otherBoardPawn);
	}
}

void UTileCentralTrigger::OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex) {
	if (auto otherBoardPawn = Cast<ABoardPawn>(otherActor); otherBoardPawn) {
		
		Tile->PawnLeftTileCenter(*otherBoardPawn);
	}
}
