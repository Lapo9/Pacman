#include "Tile.h"
#include "PacmanSettings.h"

// Sets default values
ATile::ATile() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Make this object movable in the scene
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	// Add a mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned.
void ATile::BeginPlay() {
	Super::BeginPlay();	
}


// Returns the coordinates of the center of the tile.
FVector ATile::GetCenter() const {
	FVector center = GetActorLocation();
	float floor = Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->FloorHeight;
	return FVector{ center.X, center.Y, floor}; //the height of the center is determined by the floor
}


// Returns the axis aligned extents of this tile.
FVector ATile::GetExtents() const {
	return Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()))->GetStaticMesh()->GetBounds().BoxExtent;
}
