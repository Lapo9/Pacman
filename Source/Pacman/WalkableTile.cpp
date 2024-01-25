#include "WalkableTile.h"
#include "TileCentralTrigger.h"
#include "TileFullTrigger.h"
#include "AbstractMap.h"
#include "BoardPawn.h"
#include "PacmanLevelState.h"

// Sets default values
AWalkableTile::AWalkableTile() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the triggers
	CentralTrigger = CreateDefaultSubobject<UTileCentralTrigger>(TEXT("CentralTrigger"));
	CentralTrigger->SetupAttachment(RootComponent);

	FullTrigger = CreateDefaultSubobject<UTileFullTrigger>(TEXT("FullTrigger"));
	FullTrigger->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned.
void AWalkableTile::BeginPlay() {
	Super::BeginPlay();

	// Initialize triggers
	CentralTrigger->Init(*this);
	FullTrigger->Init(*this);
}


// Called when the object is placed in the editor, or its properties change, or gameplay starts.
void AWalkableTile::OnConstruction(const FTransform& transform) {
	Super::OnConstruction(transform);
}


// Should be called by the central trigger when a pawn hit it.
void AWalkableTile::PawnOnTileCenter(ABoardPawn& pawn) const {
	pawn.OnTileCenter(*this);
}


// Should be called by the central trigger when a pawn leaves it.
void AWalkableTile::PawnLeftTileCenter(ABoardPawn& pawn) const {
	pawn.JustTeleported = false; // The pawn can be teleported again.
}


// Should be called by the full trigger when a pawn left the tile.
void AWalkableTile::PawnLeftTile(ABoardPawn& pawn) const {
	const ATile& newTile = Cast<APacmanLevelState>(GetWorld()->GetGameState())->UpdateBoardPawnTile(pawn.GetTag(), pawn.GetActorLocation()); // Update the map
	pawn.OnNewTile(newTile); // Notify the pawn
}


// Returns the type of the tile.
ETileType AWalkableTile::GetType() const {
	return ETileType::WALKABLE;
}

