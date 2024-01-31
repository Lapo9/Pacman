#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BoardPawn.generated.h"

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
	NORTH UMETA(DisplayName = "NORTH"),
	WEST UMETA(DisplayName = "WEST"),
	SOUTH UMETA(DisplayName = "SOUTH"),
	EAST UMETA(DisplayName = "EAST")
};


// Any character that can walk on the board.
UCLASS(Abstract)
class PACMAN_API ABoardPawn : public APawn
{
	GENERATED_BODY()

public:
	ABoardPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to notify to the pawn that it is at the center of the current tile.
	virtual void OnTileCenter(const class AWalkableTile& tile);

	// Called to notify the pawn that it left the center of the current tile.
	virtual void OnLeftTileCenter(const class AWalkableTile& tile);

	// Called to notify the pawn that it entered a new tile.
	virtual void OnNewTile(const class AWalkableTile& tile);

	// The pawn can now move.
	virtual void StartMoving();

	// The pawn cannot move anymore.
	virtual void StopMoving();

	// Returns the tag of this BoardPawn.
	ECharacterTag GetTag() const;
	
	// Returns the direction this pawn is moving
	EMovingDirection GetMovingDirection() const;

	// Returns the tile the pawn is onto.
	const class AWalkableTile* GetCurrentTile() const;

	// Returns the location of the central collider.
	FVector GetCentralColliderLocation() const;

	// Returns the location in 2D.
	FVector2D GetLocation2d() const;

	// Returns the tile where this BoardPawn spawns in the maze.
	const class AWalkableTile* GetSpawnTile() const;

	// Sets the new location, but leaves the Z component the same.
	void SetLocation2d(const FVector& newPos);

	// Sets the base speed of this BoardPawn.
	void SetBaseSpeed(float speed);

protected:

	// Returns the actual speed of the pawn, including the multipliers.
	virtual float GetActualSpeed(const AWalkableTile& tile) const;

	UPROPERTY(EditInstanceOnly, Category = "Pacman|Maze related") // Where the board pawn spawns
	class AWalkableTile* SpawnTile;

	UPROPERTY(EditDefaultsOnly, Category = "Pacman") // Tag identifying this BoardPawn.
	ECharacterTag Tag;

	UPROPERTY(VisibleAnywhere) // Component to place this actor on the scene.
	class USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere) // The mesh component.
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Pacman") // The default mesh.
	class UStaticMesh* DefaultMesh;

	UPROPERTY(VisibleAnywhere, Category = "Pacman|2D world") // A sphere collider representing the puntual position of this pawn.
	class USphereComponent* CentralCollider;

	// Component responsible to move the pawn.
	class UBoardPawnMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Pacman|Movement") // The standard speed of this pawn.
	float BaseSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Pacman|Movement") // How much the mode contributes to the speed of this pawn. 
	float ModeSpeedMultiplier;

	const AWalkableTile* CurrentTile; // Current tile the pawn is on.

public:
	// Whether the pawn has just been teleported (it is used to avoid infinte teleportation if the teleporting landing tile is a teleport too).
	const class ATeleportTile* TeleportedFromTile;
};
