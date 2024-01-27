#include "BoardPawnMovementComponent.h"
#include "BoardPawn.h"
#include "PacmanLevelState.h"
#include "Tile.h"
#include "PacmanUtilities.h"
#include "PacmanSettings.h"
#include "GenericPlatform/GenericPlatformMath.h"


UBoardPawnMovementComponent::UBoardPawnMovementComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}


void UBoardPawnMovementComponent::SetSpeed(float speed) {
	Speed = speed;
}


void UBoardPawnMovementComponent::SetMovingDirection(EMovingDirection dir) {
	// We have to check that, by moving the pawn in the new direction, it remains on the central axis of the tile he is onto.
	auto pawn = Cast<ABoardPawn>(GetOwner());
	auto tileCenter = pawn->GetCurrentTile()->GetCenter();
	auto pawnCenter = pawn->GetActorLocation();
	FVector distanceFromAxis = tileCenter - pawnCenter; // Distance from the center of the tile
	float tolerance = Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->PointLikeTriggersRadius * 2.f; 
	bool canMoveOnNewDirection = (dir == EMovingDirection::NORTH || dir == EMovingDirection::SOUTH) ? 
		FGenericPlatformMath::Abs(distanceFromAxis.Y) < tolerance : FGenericPlatformMath::Abs(distanceFromAxis.X) < tolerance;
	
	//TODOverifyf(canMoveOnNewDirection, 
	//		TEXT("UBoardPawnMovementComponent::SetMovingDirection: moving in the specified direction would cause exiting the movement on the grid."));
	if (!canMoveOnNewDirection) UE_LOG(LogTemp, Error, TEXT("SetMovingDirection distance: %f, %f"), distanceFromAxis.X, distanceFromAxis.Y);

	if (canMoveOnNewDirection) MovingDirection = dir;
}


EMovingDirection UBoardPawnMovementComponent::GetMovingDirection() const {
	return MovingDirection;
}


// Should be called when the pawn reaches the center of a Tile.
void UBoardPawnMovementComponent::OnTileCenter(const ATile& tile) {
	FVector tileCenter = tile.GetCenter();
	GetOwner()->GetRootComponent()->SetWorldLocation(FVector{ tileCenter.X, tileCenter.Y, GetOwner()->GetActorLocation().Z }); // Make sure the pawn is actually at the center (there may be slight variations due to approximations or colliders radius)

	// If there is an obstacle in the moving direction, stop the pawn
	auto surroundingTiles = Cast<APacmanLevelState>(GetWorld()->GetGameState())->GetSurroundingTiles(Cast<ABoardPawn>(GetOwner())->GetTag());
	if (!PacmanUtilities::IsTileWalkable(surroundingTiles[MovingDirection])) {
		Speed = 0.f;
	}
}


// Returns whether the pawn is stationary.
bool UBoardPawnMovementComponent::IsStationary() const {
	return Speed == 0.f;
}


// Moves the pawn based on speed and direction.
void UBoardPawnMovementComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) {
	auto rootComponent = GetOwner()->GetRootComponent();
	FVector dir = PacmanUtilities::MovingDirectionToVector(MovingDirection);
	FVector newPos = rootComponent->GetComponentLocation() + dir * Speed * deltaTime;
	rootComponent->SetWorldLocation(newPos);
}
