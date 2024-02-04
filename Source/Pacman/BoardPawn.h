#pragma once

#include "CoreMinimal.h"
#include "PacmanUtilities.h"
#include "GameFramework/Pawn.h"
#include "BoardPawn.generated.h"


// Forward declarations
class AWalkableTile;
class ATeleportTile;
class USphereComponent;
class UBoardPawnMovementComponent;


// Tags identifying the various characters
UENUM()
enum class ECharacterTag {
	PACMAN UMETA(DisplayName = "PACMAN"),
	BLINKY UMETA(DisplayName = "BLINKY"),
	PINKY UMETA(DisplayName = "PINKY"),
	INKY UMETA(DisplayName = "INKY"),
	CLYDE UMETA(DisplayName = "CLYDE")
};

// The 4 directions a BoardPawn can be moving.
UENUM()
enum class EMovingDirection {
	NORTH UMETA(DisplayName = "NORTH"), // {1,0,0}
	WEST UMETA(DisplayName = "WEST"), // {0,-1,0}
	SOUTH UMETA(DisplayName = "SOUTH"), // {-1,0,0}
	EAST UMETA(DisplayName = "EAST") // {0,1,0}
};


// A BoardPawn is any character that can walk on the maze.
// A BoardPawn has a sphere collider at its center, whose radius is controlled by the PointLikeTriggerRadius variable, and whose height on the Z coordinate is determined by the FloorHeight variable in PacmanSettings.
// Since the BoardPawn is logically at the same height of the WalkableTiles, it will overlap with them. The WalkableTiles will then notify the BoardPawn if he is on the center of the tile, or it he is on a new tile.
// Thanks to these notifications the BoardPawn can decide how to act based on the position on the maze he is onto. 
// BoardPawns can also be referred to as "characters" in the codebase.
UCLASS(Abstract)
class PACMAN_API ABoardPawn : public APawn, public IInitializable, public IStartableStoppable {
	GENERATED_BODY()

public:
	ABoardPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Init() override;

	virtual void Start() override;

	virtual void Stop() override;

	// Called to notify to the pawn that it is at the center of the current tile.
	virtual void OnTileCenter(const AWalkableTile& tile);

	// Called to notify the pawn that it left the center of the current tile.
	virtual void OnLeftTileCenter(const AWalkableTile& tile);

	// Called to notify the pawn that it entered a new tile.
	virtual void OnNewTile(const AWalkableTile& tile);

	// Returns the tag of this BoardPawn.
	ECharacterTag GetTag() const;
	
	// Returns the direction this pawn is moving
	EMovingDirection GetMovingDirection() const;

	// Returns the tile the pawn is onto.
	const AWalkableTile* GetCurrentTile() const;

	// Returns the location of the central collider.
	FVector GetCentralColliderLocation() const;

	// Returns the location in 2D.
	FVector2D GetLocation2d() const;

	// Returns the tile where this BoardPawn spawns in the maze.
	const AWalkableTile* GetSpawnTile() const;

	// Sets the new location, but leaves the Z component the same.
	void SetLocation2d(const FVector& newPos);

	// Sets the base speed of this BoardPawn.
	void SetBaseSpeed(float speed);

protected:
	// Returns the actual speed of the pawn, including the multipliers.
	virtual float GetActualSpeed(const AWalkableTile& tile) const;

	UPROPERTY(EditInstanceOnly, Category = "Pacman|Maze related") // Where the board pawn spawns
	AWalkableTile* SpawnTile;

	UPROPERTY(EditAnywhere, Category = "Pacman") // Tag identifying this BoardPawn.
	ECharacterTag Tag;

	UPROPERTY(VisibleAnywhere) // Component to place this actor on the scene.
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere) // The mesh component.
	USkeletalMeshComponent* Mesh;

	USkeletalMesh* DefaultMesh; // The default mesh.
	TArray<UMaterialInterface*> DefaultMaterials; // The default material.

	UPROPERTY(VisibleAnywhere, Category = "Pacman|2D world") // A sphere collider representing the puntual position of this pawn.
	USphereComponent* CentralCollider;

	UPROPERTY(EditAnywhere, Category = "Pacman") // The collider for the full board pawn.
	USphereComponent* FullCollider;

	// Component responsible to move the pawn.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoardPawnMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Pacman|Movement") // The standard speed of this pawn.
	float BaseSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Pacman|Movement") // How much the mode contributes to the speed of this pawn. 
	float ModeSpeedMultiplier;

	const AWalkableTile* CurrentTile; // Current tile the pawn is on.

public:
	// Whether the pawn has just been teleported (it is used to avoid infinte teleportation if the teleporting landing tile is a teleport too).
	const ATeleportTile* TeleportedFromTile;
};
