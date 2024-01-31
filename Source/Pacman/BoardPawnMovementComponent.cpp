#include "BoardPawnMovementComponent.h"
#include "BoardPawn.h"
#include "PacmanLevelState.h"
#include "Tile.h"
#include "TeleportTile.h"
#include "PacmanUtilities.h"
#include "PacmanSettings.h"
#include "TileCentralTrigger.h"
#include "WalkableTile.h"
#include "UObject/Class.h"
#include "GenericPlatform/GenericPlatformMath.h"


UBoardPawnMovementComponent::UBoardPawnMovementComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UBoardPawnMovementComponent::BeginPlay() {
	PointLikeTolerance = Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->PointLikeTriggersRadius;
}


void UBoardPawnMovementComponent::SetSpeed(float speed) {
	Speed = speed;
}


// Sets the new moving direction, but also make sure the pawn remains on the "rails" of the maze (returns false and does nothing if it doesn't).
bool UBoardPawnMovementComponent::SetMovingDirection(EMovingDirection dir) {
	// If the pawn missed a hit in the last frame, now it will be further away from the center than 2 radii, but it should still be possible to change direction
	bool canMoveOnNewDirection = OnTileCenterInfo.ConsumeAllowAnyChangeOfDirection();

	if (!canMoveOnNewDirection) {
		// We have to check that, by moving the pawn in the new direction, it remains on the central axis of the tile he is onto.
		auto pawn = Cast<ABoardPawn>(GetOwner());
		auto tileCenter = pawn->GetCurrentTile()->GetLocation2d();
		auto pawnCenter = pawn->GetLocation2d();
		FVector2D distanceFromAxis = tileCenter - pawnCenter; // Distance of the pawn from the center of the tile

		canMoveOnNewDirection = (dir == EMovingDirection::NORTH || dir == EMovingDirection::SOUTH) ?
			FGenericPlatformMath::Abs(distanceFromAxis.Y) < PointLikeTolerance * 2.f : FGenericPlatformMath::Abs(distanceFromAxis.X) < PointLikeTolerance * 2.f;
		
		if (!canMoveOnNewDirection) { UE_LOG(LogTemp, Error, TEXT("Cannot move %s in direction %s; distance from %s center: <%f, %f>"), *(pawn->GetName()), *UEnum::GetValueAsString<EMovingDirection>(dir), *(pawn->GetCurrentTile()->GetName()), distanceFromAxis.X, distanceFromAxis.Y); }
	}

	if(canMoveOnNewDirection) MovingDirection = dir;
	return canMoveOnNewDirection;
}


EMovingDirection UBoardPawnMovementComponent::GetMovingDirection() const {
	return MovingDirection;
}


// Should be called when the pawn reaches the center of a Tile.
void UBoardPawnMovementComponent::OnTileCenter(const AWalkableTile& tile) {
	if (OnTileCenterInfo.SkipNextOnTileCenter) return;

	// Fill the OnCenterInfo, so that the movement can be managed during the next TickComponent.
	OnTileCenterInfo.IsOnTileCenter = true;
	OnTileCenterInfo.Center = tile.GetLocation2d();

	auto deltaToCenter = OnTileCenterInfo.Center - Cast<ABoardPawn>(GetOwner())->GetLocation2d(); // Vector from pawn to tile center
	auto movDir = FVector2D{ Util::MovingDirectionToVector(MovingDirection) };
	FVector2D deltaToCenterRelativeToMovDirection = deltaToCenter * movDir; // Vector from pawn to tile center in the coordinates system of the pawn (i.e. positive if the pawn has the center in front of him)
	int dirToCenter = deltaToCenterRelativeToMovDirection.X > 0 ? 1 : deltaToCenterRelativeToMovDirection.X < 0 ? -1 : deltaToCenterRelativeToMovDirection.Y > 0 ? 1 : -1; // 1 if the pawn is "before" the center, -1 if it is after
	OnTileCenterInfo.DistFromCenter = deltaToCenter.GetAbsMax() * dirToCenter;

	// If there is an obstacle in the moving direction, stop the pawn
	auto surroundingTiles = Cast<APacmanLevelState>(GetWorld()->GetGameState())->GetSurroundingTiles(*Cast<ABoardPawn>(GetOwner()));
	if (!PacmanUtilities::IsTileWalkable(surroundingTiles[MovingDirection])) {
		Speed = 0.f;
	}
}


// Should be called when the pawn leaves the center of a Tile.
void UBoardPawnMovementComponent::OnLeftTileCenter() {
	OnTileCenterInfo.SkipNextOnTileCenter = false; // See comments in OnTileCenterRecovery and OnTileCenter
}


// Returns whether the pawn is stationary.
bool UBoardPawnMovementComponent::IsStationary() const {
	return Speed == 0.f;
}


// Calculates the delta movement of the pawn in the last frame. If there was an overlap with a tile center in the last frame, the returned delta movement is considered from the center of the overlapped tile (not the actual position of the pawn)
FVector UBoardPawnMovementComponent::ComputeDeltaMovement(float deltaTime) const {
	if (IsStationary()) return FVector::Zero();
	
	float coveredDistance = Speed * deltaTime;
	// Handle the case there there was an overlap with the center of a tile in the past frame.
	if (OnTileCenterInfo.IsOnTileCenter) {
		coveredDistance -= OnTileCenterInfo.DistFromCenter; // The covered distance in the new direction will be the total covered distance - the signed distance to get to the center
	}

	return Util::MovingDirectionToVector(MovingDirection) * coveredDistance;
}


// Detects if there was an overlap with a tile center that wasn't detected by Unreal Engine because the pawn skipped over the tile central trigger due to a too high delta time between 2 frames.
FVector UBoardPawnMovementComponent::OnTileCenterRecovery(float deltaTime, FVector delta) {
	// If the movement was longer than 4 point-like radii (plural of radius), we may have missed a hit (unless we got a hit in this frame, in which case it is impossible we missed anything)
	if (delta.GetAbsMax() >= PointLikeTolerance * 4.f && !OnTileCenterInfo.IsOnTileCenter) {
		UE_LOG(LogTemp, Warning, TEXT("Last frame-time was too long ( %f ), %s moved too much and a collision with a central trigger may have been lost. Delta movement: <%f, %f>"), deltaTime, *GetOwner()->GetName(), delta.X, delta.Y);

		// Perform a ray-trace to see if the center of the pawn hit something during the last movement
		TArray<FHitResult> hits;
		FVector centralColliderLocation = Cast<ABoardPawn>(GetOwner())->GetCentralColliderLocation() + delta;
		FVector movDir = Util::MovingDirectionToVector(MovingDirection);
		FVector rayEnd = centralColliderLocation - delta + movDir * PointLikeTolerance * 3.f;
		GetWorld()->LineTraceMultiByChannel(hits, centralColliderLocation, rayEnd, ObjectChannel_World2d);
		// Why * 3.f? The Worst case scenario to miss a hit is when, in the last frame, the pawn got very close to hitting the centre:
		//   old    tile centre          new pawn pos
		// (  +  )(  +  )               (  +  )
		//    |---------|<-----------------|
		//     3 radii  ^--we get till here with our ray cast

		// Find the first hit against a TileCentralTrigger
		const FHitResult* hitWithCentralTrigger = hits.FindByPredicate([](const FHitResult& hit) { return hit.GetComponent()->IsA(UTileCentralTrigger::StaticClass()); });
		
		// If there is a hit against a TileCentralTrigger...
		if (hitWithCentralTrigger != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Recovering missed hit during last frame of %s"), *GetOwner()->GetName());
			const AWalkableTile* tile = Cast<AWalkableTile>(hitWithCentralTrigger->GetActor()); // Get the tile owning the central trigger (it will always be a walkable tile)
			OnTileCenterInfo.AllowAnyChangeOfDirection = true; // To let SetMovingDirection change direction even if we are too far away from the tile center
			tile->PawnOnTileCenter(*Cast<ABoardPawn>(GetOwner())); // Invoke collision handling with that tile (it will compute new speed and direction)
			delta = ComputeDeltaMovement(deltaTime); // Re-compute delta movement

			// If the movement is bigger than 2 radii, it means we are not overlapping with the center anymore, therefore the next overlap with a tile center is "real". 
			// Else, the next overlap is spurious, and it will happen next frame, because UE doesn't know that we already overlapped during the current frame. The mode will be concluded when the pawn exits the tile central trigger (OnLeftTileCenter).
			if (delta.GetAbsMax() > PointLikeTolerance * 2.f) {
				OnTileCenterInfo.SkipNextOnTileCenter = false;
				tile->PawnLeftTileCenter(*Cast<ABoardPawn>(GetOwner()));
			}
			else {
				OnTileCenterInfo.SkipNextOnTileCenter = true;		
			}
		}
	}
	return delta;
}


// Moves the pawn based on speed and direction.
void UBoardPawnMovementComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) {
	if (!CanMove) return;

	auto rootComponent = GetOwner()->GetRootComponent();
	FVector delta = ComputeDeltaMovement(deltaTime);
	delta = OnTileCenterRecovery(deltaTime, delta); // Check whether we missed a hit due to big deltaTime, and react to it.

	auto currentPawnPosition = rootComponent->GetComponentLocation();
	// If the pawn got in the center of the tile during last frame, ComputeDeltaMovement will give back the delta from the center of the tile, therefore we have to make the starting point of the movement the center of the tile.
	if (OnTileCenterInfo.IsOnTileCenter) {
		OnTileCenterInfo.IsOnTileCenter = false;
		currentPawnPosition = FVector{ OnTileCenterInfo.Center, currentPawnPosition.Z };
	}
	FVector newPos =  currentPawnPosition + delta;
	rootComponent->SetWorldLocation(newPos);
}


