#pragma once

#include "CoreMinimal.h"
#include "BoardPawn.h"
#include "Components/ActorComponent.h"
#include "BoardPawnMovementComponent.generated.h"


// Component to move the BoardPawns in the maze.
// The component is designed to accomplish 3 tasks: 
// 1) Move the pawns.
// 2) Make sure they remain on the "rails" of the maze, regardless of the input.
//	To accomplish this goal, the function SetMovingDirection will check that the new direction doesn't move the pawn out of its "rail".
// 3) Deal with potential drops in the frame rate of the game.
//	If the frame rate is too low, it is possible that, when we move the pawn, we miss an overlap with a TileCentralTrigger.
//	Using the sweep mode developed by Unreal Engine 5.2.3 is not useful, because the sweep is done only for the root component of the pawn, but we need to check for a collision of the BoardPawn central collider instead.
//	To handle this, if the delta time between 2 frames is too high, we perform a ray-trace to find out if we missed an overlap. In case we did, the overlap event for the tile is manually invoked, and the new movement is computed.
//	Of course this should happen rarely, in case it happens too often it is better to enlarge the radius of the triggers (if the radius is >= 1/FPS * maxSpeed this never happens).
//	Moreover, if the frame time so low that we miss 2 or more TileCentralTriggers in one frame, our method may fail.
UCLASS()
class PACMAN_API UBoardPawnMovementComponent : public UActorComponent {
	GENERATED_BODY()
	
public:
	UBoardPawnMovementComponent();

	void BeginPlay() override;

	// Moves the pawn based on speed and direction.
	virtual void TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

	virtual void SetSpeed(float speed);

	// Sets the new moving direction, but also make sure the pawn remains on the "rails" of the maze (returns false and does nothing if it doesn't).
	virtual bool SetMovingDirection(EMovingDirection dir);
	virtual EMovingDirection GetMovingDirection() const;

	// Should be called when the pawn reaches the center of a Tile.
	virtual void OnTileCenter(const class AWalkableTile& tile);

	// Should be called when the pawn leaves the center of a Tile.
	virtual void OnLeftTileCenter();

	// Returns whether the pawn is stationary.
	virtual bool IsStationary() const;

protected:
	// Calculates the movement of the pawn.
	virtual FVector ComputeDeltaMovement(float deltaTime) const;

	// Detects if there was an overlap with a tile center that wasn't detected by Unreal Engine because the pawn skipped over the tile central trigger due to a too high delta time between 2 frames.
	virtual FVector OnTileCenterRecovery(float deltaTime, FVector delta);

	UPROPERTY(VisibleAnywhere, Category = "Real-time info") // Direction where the pawn is moving.
	EMovingDirection MovingDirection;

	UPROPERTY(VisibleAnywhere, Category = "Real-time info") // The current speed of the pawn.
	float Speed;

	// Central triggers represent points, but are little spheres: this is 2 times the radius of these spheres.
	float PointLikeTolerance;

	// Info populated when the function OnTileCenter is called.
	struct {
		bool IsOnTileCenter; // True during the frame in which OnTileCenter has been called.
		bool SkipNextOnTileCenter; // When true, OnTileCenter will do nothing. Set when the tick function detects that an overlap with a center was missed during the last frame.
		bool AllowAnyChangeOfDirection; // Used by SetMovingDirection. Set when the tick function detects that an overlap with a center was missed during the last frame.
		FVector2D Center; // If IsOnTileCenter is true, holds the position of the tile whose center was hit.
		float DistFromCenter; // If IsOnTileCenter is true, holds the distance of the pawn to the center of the tile.

		inline bool ConsumeTileCenter() {
			bool res = IsOnTileCenter;
			IsOnTileCenter = false;
			return res;
		}

		inline bool ConsumeAllowAnyChangeOfDirection() {
			bool res = AllowAnyChangeOfDirection;
			AllowAnyChangeOfDirection = false;
			return res;
		}

	} OnTileCenterInfo;

public:
	UPROPERTY(VisibleAnywhere, Category = "Real-time info")
	bool CanMove; // Whether the pawn can move or not.
};
