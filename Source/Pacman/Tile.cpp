#include "Tile.h"
#include "PacmanSettings.h"
#include "PacmanLevelState.h"


ATile::ATile() : ATile{ ETileType::WALKABLE } {
	// This ctor is mandatory by UE
}


ATile::ATile(ETileType type) : TileType { type } {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Make this object movable in the scene
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	// Add a mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionObjectType(ObjectChannel_Walls); // BoardPawns object channel
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Disable all collisions
	Mesh->SetGenerateOverlapEvents(false); // Completely disable overlap events
}


// Called when the game starts or when spawned.
void ATile::BeginPlay() {
	UE_LOG(LogTemp, Display, TEXT("BeginPlay tile %s"), *GetName());
	auto& levelState = *Cast<APacmanLevelState>(GetWorld()->GetGameState());
	levelState.RegisterForInitialization(*this); // Register to receive the initialization call
	Super::BeginPlay();
}


void ATile::Init() {
	UE_LOG(LogTemp, Display, TEXT("Initializing tile %s"), *GetName());
}


// Returns the coordinates of the center of the tile.
FVector ATile::GetCenter() const {
	FVector center = GetActorLocation();
	float floor = Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->FloorHeight;
	return FVector{ center.X, center.Y, floor}; //the height of the center is determined by the floor
}


// Returns the coordinates of the center of the tile (only X and Y).
FVector2D ATile::GetLocation2d() const {
	return FVector2D{ GetActorLocation() };
}


// Returns the axis aligned extents of this tile.
FVector ATile::GetExtents() const {
	if (Mesh == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Mesh of %s is nullptr"), *GetName());
		return FVector{ 50.f,50.f,10.f }; // TODO
	}
	FVector min, max;
	Mesh->GetLocalBounds(min, max);
	return (max - min) / 2.f;
}


// Returns the type of the tile.
ETileType ATile::GetType() const {
	return TileType;
}
