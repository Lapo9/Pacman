#include "WalkableTile.h"
#include "TileCentralTrigger.h"
#include "TileFullTrigger.h"
#include "AbstractMap.h"
#include "BoardPawn.h"
#include "PacmanLevelState.h"
#include "StandardFood.h"


AWalkableTile::AWalkableTile() : AWalkableTile{ ETileType::WALKABLE } {
	// Reuired by UE
}

// Sets default values
AWalkableTile::AWalkableTile(ETileType type) : Super{ type } {
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

	auto& levelState = *Cast<APacmanLevelState>(GetWorld()->GetGameState());
	levelState.RegisterForStartAndStop(*this); // Register to receive the start call

	// Initialize triggers
	CentralTrigger->Init(*this);
	FullTrigger->Init(*this);
}


void AWalkableTile::Init() {
	Super::Init();
	// Delete old food (if present)
	if (SpawnedFood && SpawnedFood->GetName() != TEXT("none")) SpawnedFood->Destroy(); 
	// TODO Idk why but sometimes the food seems in a non valid state, and calling destroy on it crashes the game.
	// A call on IsBeingDestroyed or IsValidLowLevel doesn't catch this problem, but the food name is none if it is in this weird state.
}


void AWalkableTile::Start() {
	// Spawn new food
	SpawnFood();
}


void AWalkableTile::Stop() {
}


// Should be called by the central trigger when a pawn hit it.
void AWalkableTile::PawnOnTileCenter(ABoardPawn& pawn) const {
	pawn.OnTileCenter(*this); // Notify the pawn
}


// Should be called by the central trigger when a pawn leaves it.
void AWalkableTile::PawnLeftTileCenter(ABoardPawn& pawn) const {
	pawn.OnLeftTileCenter(*this); // Notify the pawn
}


// Should be called by the full trigger when a pawn left the tile.
void AWalkableTile::PawnLeftTile(ABoardPawn& pawn) const {
	const AWalkableTile* newTile = Cast<APacmanLevelState>(GetWorld()->GetGameState())->UpdateBoardPawnTile(pawn, pawn.GetActorLocation()); // Update the map
	pawn.OnNewTile(newTile); // Notify the pawn
}


// Returns the value to increase or decrease the speed of a BoardPawn on this tile.
float AWalkableTile::GetSpeedMultiplier() const {
	return SpeedMultiplier;
}


// Spawns the food item (if required).
void AWalkableTile::SpawnFood() const {
	if (!FoodToSpawn) return;
	FTransform transform{ GetActorRotation(), GetActorLocation() + FVector{ 0,0, DistanceFromTile }, GetActorScale() }; // Spawn above the tile
	SpawnedFood = GetWorld()->SpawnActor<ABaseFood>(FoodToSpawn->GetAuthoritativeClass(), transform);
}
