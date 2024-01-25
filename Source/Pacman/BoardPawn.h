#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BoardPawn.generated.h"

// Tags identifying the various characters
UENUM()
enum class ECharacterTag { PACMAN, BLINKY, PINKY, INKY, CLYDE };


// The 4 directions a BoardPawn can be moving.
UENUM()
enum class EMovingDirection { NORTH, EAST, SOUTH, WEST };


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
	virtual void OnTileCenter(const class ATile& tile) PURE_VIRTUAL(ABoardPawn::OnTileCenter, );

	// Called to notify the pawn that it entered a new tile.
	virtual void OnNewTile(const class ATile& tile);

	// Returns the tag of this BoardPawn.
	ECharacterTag GetTag() const;
	
	// Returns the tile the pawn is onto.
	const class ATile* GetCurrentTile() const;

	// Sets the new location, but leaves the Z component the same.
	void SetLocation2D(const FVector& newPos);

protected:
	UPROPERTY(VisibleAnywhere) // Tag identifying this BoardPawn.
	ECharacterTag Tag;

	UPROPERTY(VisibleAnywhere) // Component to place this actor on the scene.
	class USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere) // The mesh.
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere) // A sphere collider representing the puntual position of this pawn.
	class USphereComponent* CentralCollider;

	UPROPERTY(VisibleAnywhere) // Component responsible to move the pawn.
	class UBoardPawnMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere) // The standard speed of this pawn.
	float StandardSpeed;

	UPROPERTY(EditAnywhere)// The speed of the pawn inside the tunnel.
	float TunnelSpeed;

	// Current tile the pawn is on.
	const ATile* CurrentTile;

public:
	// Whether the pawn has just been teleported (it is used to avoid infinte teleportation if the teleporting landing tile is a teleport too).
	bool JustTeleported;
};
