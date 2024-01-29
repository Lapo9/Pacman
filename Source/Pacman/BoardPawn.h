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
	// Sets default values for this pawn's properties
	ABoardPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to notify the pawn that it is at the center of the current tile.
	virtual void OnTileCenter(const class AWalkableTile& tile) PURE_VIRTUAL(ABoardPawn::OnTileCenter, );

	// Called to notify the pawn that it entered a new tile.
	virtual void OnNewTile(const class AWalkableTile& tile);

	// Returns the tag of this BoardPawn.
	ECharacterTag GetTag() const;
	
	// Returns the direction this pawn is moving
	EMovingDirection GetMovingDirection() const;

	// Returns the tile the pawn is onto.
	const class AWalkableTile* GetCurrentTile() const;

	// Returns the location of the central collider.
	FVector GetCentralColliderLocation() const;

	// Sets the new location, but leaves the Z component the same.
	void SetLocation2d(const FVector& newPos);

	// Returns the location in 2D.
	FVector2D GetLocation2d() const;

	const class AWalkableTile* GetSpawnTile() const;

protected:
	UPROPERTY(EditAnywhere, Category = "Initialization parameters") // Where the board pawn spawns
	class AWalkableTile* SpawnTile;

	UPROPERTY(EditAnywhere, Category = "Initialization parameters") // Tag identifying this BoardPawn.
	ECharacterTag Tag;

	UPROPERTY(VisibleAnywhere) // Component to place this actor on the scene.
	class USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere) // The mesh component.
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere) // The default mesh.
	class UStaticMesh* DefaultMesh;

	UPROPERTY(VisibleAnywhere) // A sphere collider representing the puntual position of this pawn.
	class USphereComponent* CentralCollider;

	UPROPERTY(VisibleAnywhere) // Component responsible to move the pawn.
	class UBoardPawnMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere) // The standard speed of this pawn.
	float StandardSpeed;

	UPROPERTY(EditAnywhere)// The speed of the pawn inside the tunnel.
	float TunnelSpeed;

	// Current tile the pawn is on.
	const AWalkableTile* CurrentTile;

public:
	// Whether the pawn has just been teleported (it is used to avoid infinte teleportation if the teleporting landing tile is a teleport too).
	bool JustTeleported;
};
