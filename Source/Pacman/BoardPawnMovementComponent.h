#pragma once

#include "CoreMinimal.h"
#include "BoardPawn.h"
#include "Components/ActorComponent.h"
#include "BoardPawnMovementComponent.generated.h"


UCLASS()
class PACMAN_API UBoardPawnMovementComponent : public UActorComponent {
	GENERATED_BODY()
	
public:
	UBoardPawnMovementComponent();

	// Moves the pawn based on speed and direction.
	virtual void TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

	virtual void SetSpeed(float speed);

	virtual void SetMovingDirection(EMovingDirection dir);
	virtual EMovingDirection GetMovingDirection() const;

	// Should be called when the pawn reaches the center of a Tile.
	virtual void OnTileCenter(const class ATile& tile);

	// Returns whether the pawn is stationary.
	virtual bool IsStationary() const;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Real-time info") // Direction where the pawn is moving.
	EMovingDirection MovingDirection;

	UPROPERTY(VisibleAnywhere, Category = "Real-time info") // The current speed of the pawn.
	float Speed;
};
