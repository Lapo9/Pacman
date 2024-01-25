#include "BoardPawn.h"
#include "Tile.h"
#include "BoardPawnMovementComponent.h"
#include "PacmanSettings.h"
#include "Components/SphereComponent.h"
#include "PacmanSettings.h"

// Sets default values
ABoardPawn::ABoardPawn() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Make this object movable in the scene
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	// Add a mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	// Add the central collider
	CentralCollider = CreateDefaultSubobject<USphereComponent>(TEXT("CentralCollider"));
	CentralCollider->SetupAttachment(RootComponent);

	// Add the movement component
	MovementComponent = CreateDefaultSubobject<UBoardPawnMovementComponent>(TEXT("MovementComponent"));
}


// Called when the game starts or when spawned
void ABoardPawn::BeginPlay() {
	Super::BeginPlay();

	CurrentTile = Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->SpawnTiles[Tag]; //Initialize the current tile
	SetLocation2D(CurrentTile->GetCenter()); // Move the pawn to the spawn tile center (don't touch Z component)
	
	// Set the position of the trigger, and its radius
	auto center = GetActorLocation();
	CentralCollider->SetWorldLocation(FVector{ center.X, center.Y, Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->FloorHeight }); //Set the z height to the logical floor
	CentralCollider->SetSphereRadius(Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->PointLikeTriggersRadius);

}


// Called to notify the pawn that it entered a new tile.
void ABoardPawn::OnNewTile(const ATile& tile) {
	// Register the new tile and change speed.
	CurrentTile = &tile;
	MovementComponent->SetSpeed(tile.GetType() == ETileType::TUNNEL ? TunnelSpeed : StandardSpeed);
}


// Returns the tag of this BoardPawn.
ECharacterTag ABoardPawn::GetTag() const {
	return Tag;
}


// Returns the tile the pawn is onto.
const ATile* ABoardPawn::GetCurrentTile() const {
	return CurrentTile;
}


// Sets the new location, but leaves the Z component the same.
void ABoardPawn::SetLocation2D(const FVector& newPos) {
	SetActorLocation(FVector{ newPos.X, newPos.Y, GetActorLocation().Z });
}


