#include "BoardPawn.h"
#include "WalkableTile.h"
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

	// Add the central collider and set up collision rules
	CentralCollider = CreateDefaultSubobject<USphereComponent>(TEXT("CentralCollider"));
	CentralCollider->SetupAttachment(RootComponent);
	CentralCollider->SetCollisionObjectType(ObjectChannel_World2d); // World2d object channel
	CentralCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Disable all collisions
	CentralCollider->SetCollisionResponseToChannel(ObjectChannel_World2d, ECollisionResponse::ECR_Overlap); // enable World2d object channel
	
	// Add a mesh and set up mesh collision rules
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionObjectType(ObjectChannel_BoardPawns); // BoardPawns object channel
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Disable all collisions
	Mesh->SetCollisionResponseToChannel(ObjectChannel_BoardPawns, ECollisionResponse::ECR_Overlap); // enable BoardPawns object channel
	Mesh->SetCollisionResponseToChannel(ObjectChannel_Walls, ECollisionResponse::ECR_Overlap); // enable Walls object channel

	// Add the movement component
	MovementComponent = CreateDefaultSubobject<UBoardPawnMovementComponent>(TEXT("MovementComponent"));
}


// Called when the game starts or when spawned
void ABoardPawn::BeginPlay() {
	UE_LOG(LogTemp, Display, TEXT("BeginPlay board pawn %s"), *GetName());
	Super::BeginPlay();

	CurrentTile = SpawnTile; //Initialize the current tile
	SetLocation2d(CurrentTile->GetCenter()); // Move the pawn to the spawn tile center (don't touch Z component)
	
	// Set the position of the trigger, and its radius
	auto center = GetActorLocation();
	CentralCollider->SetWorldLocation(FVector{ center.X, center.Y, Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->FloorHeight }); //Set the z height to the logical floor
	CentralCollider->SetSphereRadius(Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->PointLikeTriggersRadius);

	// Call the first event to make the pawn start moving. TODO maybe there is a better way to start moving
	OnTileCenter(*SpawnTile);
}


void ABoardPawn::OnTileCenter(const AWalkableTile& tile) {
	UE_LOG(LogTemp, Display, TEXT("Board pawn %s on tile center %s"), *GetName(), *tile.GetName());
}


// Called to notify the pawn that it left the center of the current tile.
void ABoardPawn::OnLeftTileCenter(const AWalkableTile& tile) {
	TeleportedFromTile = nullptr; // The pawn can be teleported again.
	MovementComponent->OnLeftTileCenter(); // Notify the movement component that the pawn left the tile center (it uses this event to handle some logic of spurious on tile center events)
}

// Called to notify the pawn that it entered a new tile.
void ABoardPawn::OnNewTile(const AWalkableTile& tile) {
	UE_LOG(LogTemp, Display, TEXT("Board pawn %s on new tile %s"), *GetName(), *tile.GetName());
	// Register the new tile and change speed.
	CurrentTile = &tile;
	MovementComponent->SetSpeed(tile.GetType() == ETileType::TUNNEL ? TunnelSpeed : StandardSpeed);
}


// Returns the tag of this BoardPawn.
ECharacterTag ABoardPawn::GetTag() const {
	return Tag;
}


// Returns the direction this pawn is moving
EMovingDirection ABoardPawn::GetMovingDirection() const {
	return MovementComponent->GetMovingDirection();
}


// Returns the tile the pawn is onto.
const AWalkableTile* ABoardPawn::GetCurrentTile() const {
	return CurrentTile;
}


// Returns the location of the central collider.
FVector ABoardPawn::GetCentralColliderLocation() const {
	return CentralCollider->GetComponentLocation();
}


// Sets the new location, but leaves the Z component the same.
void ABoardPawn::SetLocation2d(const FVector& newPos) {
	SetActorLocation(FVector{ newPos.X, newPos.Y, GetActorLocation().Z });
}


FVector2D ABoardPawn::GetLocation2d() const {
	return FVector2D{ CentralCollider->GetComponentLocation() };
}


const AWalkableTile* ABoardPawn::GetSpawnTile() const {
	return SpawnTile;
}


