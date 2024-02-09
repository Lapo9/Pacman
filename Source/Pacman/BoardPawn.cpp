#include "BoardPawn.h"
#include "WalkableTile.h"
#include "BoardPawnMovementComponent.h"
#include "PacmanSettings.h"
#include "PacmanLevelState.h"
#include "Components/SphereComponent.h"
#include "PacmanSettings.h"

// Sets default values
ABoardPawn::ABoardPawn() : ModeSpeedMultiplier{ 1 } {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Make this object movable in the scene
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	// Add the central and full colliders and set up collision rules
	CentralCollider = CreateDefaultSubobject<USphereComponent>(TEXT("CentralCollider"));
	CentralCollider->SetupAttachment(RootComponent);
	CentralCollider->SetCollisionObjectType(ObjectChannel_World2d); // World2d object channel
	CentralCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Disable all collisions
	CentralCollider->SetCollisionResponseToChannel(ObjectChannel_World2d, ECollisionResponse::ECR_Overlap); // enable World2d object channel
	
	FullCollider = CreateDefaultSubobject<USphereComponent>(TEXT("FullCollider"));
	FullCollider->SetupAttachment(RootComponent);
	FullCollider->SetCollisionObjectType(ObjectChannel_BoardPawns); // BoardPawns object channel
	FullCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Disable all collisions
	FullCollider->SetCollisionResponseToChannel(ObjectChannel_BoardPawns, ECollisionResponse::ECR_Overlap); // enable BoardPawns object channel
	FullCollider->SetCollisionResponseToChannel(ObjectChannel_Walls, ECollisionResponse::ECR_Overlap); // enable Walls object channel

	// Add a mesh and set up mesh collision rules
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	// Add the movement component
	MovementComponent = CreateDefaultSubobject<UBoardPawnMovementComponent>(TEXT("MovementComponent"));
}


// Called when the game starts or when spawned
void ABoardPawn::BeginPlay() {
	UE_LOG(LogTemp, Display, TEXT("BeginPlay board pawn %s"), *GetName());

	// Store the default mesh and material
	DefaultMesh = Mesh->GetSkeletalMeshAsset();
	DefaultMaterials = Mesh->GetMaterials();
	
	// Set the position of the trigger, and its radius
	auto center = GetActorLocation();
	CentralCollider->SetWorldLocation(FVector{ center.X, center.Y, Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->FloorHeight }); //Set the z height to the logical floor
	CentralCollider->SetSphereRadius(Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->PointLikeTriggersRadius);

	// Disable triggers
	CentralCollider->SetGenerateOverlapEvents(false);
	FullCollider->SetGenerateOverlapEvents(false);
	MovementComponent->bCanMove = false; // Don't move

	auto& levelState = *Cast<APacmanLevelState>(GetWorld()->GetGameState());
	levelState.RegisterForInitialization(*Cast<IInitializable>(this)); // Register to receive the initialization call
	levelState.RegisterForStartAndStop(*Cast<IStartableStoppable>(this)); // Register to receive the start call
	levelState.RegisterBoardPawn(*this);

	Super::BeginPlay();
}


void ABoardPawn::Init() {
	UE_LOG(LogTemp, Display, TEXT("Initializing board pawn %s"), *GetName());
	CurrentTile = SpawnTile;
	TeleportedFromTile = nullptr;
	ModeSpeedMultiplier = 1.f;
	Mesh->SetSkeletalMesh(DefaultMesh);
	for (int i = 0; auto material : DefaultMaterials) Mesh->SetMaterial(i++, material);

	// Disable triggers
	CentralCollider->SetGenerateOverlapEvents(false);
	FullCollider->SetGenerateOverlapEvents(false);
	MovementComponent->bCanMove = false;

	SetLocation2d(CurrentTile->GetCenter()); // Move the pawn to the spawn tile center (don't touch Z component)
}


void ABoardPawn::Start() {
	UE_LOG(LogTemp, Display, TEXT("Starting board pawn %s"), *GetName());
	// Enable triggers
	MovementComponent->bCanMove = true;
	CentralCollider->SetGenerateOverlapEvents(true);
	FullCollider->SetGenerateOverlapEvents(true);
}


void ABoardPawn::Stop() {
	// Disable triggers
	CentralCollider->SetGenerateOverlapEvents(false);
	FullCollider->SetGenerateOverlapEvents(false);
	MovementComponent->bCanMove = false;
}


// Called to notify to the pawn that it is at the center of the current tile.
void ABoardPawn::OnTileCenter(const AWalkableTile& tile) {
	UE_LOG(LogTemp, Display, TEXT("Board pawn %s on tile center %s"), *GetName(), *tile.GetName());
	MovementComponent->OnTileCenter(tile); // Notify to the movement component that the pawn reached the center of the tile
}


// Called to notify the pawn that it left the center of the current tile.
void ABoardPawn::OnLeftTileCenter(const AWalkableTile& tile) {
	TeleportedFromTile = nullptr; // The pawn can be teleported again.
	MovementComponent->OnLeftTileCenter(); // Notify the movement component that the pawn left the tile center (it uses this event to handle some logic of spurious OnTileCenter events)
}


// Called to notify the pawn that it entered a new tile.
void ABoardPawn::OnNewTile(const AWalkableTile* tile) {
	UE_LOG(LogTemp, Display, TEXT("Board pawn %s on new tile %s"), *GetName(), *tile->GetName());
	// If the new tile is null, it means there was a problem, therefore go back to the last tile
	if (!tile) SetLocation2d(CurrentTile->GetCenter());
	// Register the new tile and change speed.
	else CurrentTile = tile;

	MovementComponent->SetSpeed(GetActualSpeed(*tile));
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
	CurrentTile = Cast<AWalkableTile>(Cast<APacmanLevelState>(GetWorld()->GetGameState())->GetAbstractMap().PositionToTile(newPos));
	verifyf(CurrentTile != nullptr, TEXT("Board pawn %s teleported to non-walkable tile"), *GetName());
	Cast<APacmanLevelState>(GetWorld()->GetGameState())->UpdateBoardPawnTile(*this, newPos);
	OnNewTile(CurrentTile);
	SetActorLocation(FVector{ newPos.X, newPos.Y, GetActorLocation().Z });
}


// Returns the location in 2D.
FVector2D ABoardPawn::GetLocation2d() const {
	return FVector2D{ CentralCollider->GetComponentLocation() };
}


// Returns the tile where this BoardPawn spawns in the maze.
const AWalkableTile* ABoardPawn::GetSpawnTile() const {
	return SpawnTile;
}


// Sets the settings for this ghost.
void ABoardPawn::SetBaseSpeed(float speed) {
	BaseSpeed = speed;
}


// Returns the actual speed of the pawn, including the multipliers.
float ABoardPawn::GetActualSpeed(const AWalkableTile& tile) const {
	return BaseSpeed * tile.GetSpeedMultiplier() * ModeSpeedMultiplier;
}


