#pragma once

#include "CoreMinimal.h"
#include "BoardPawn.h"
#include "GhostModeData.h"
#include "GhostPawn.generated.h"


UCLASS()
class PACMAN_API AGhostPawn : public ABoardPawn
{
	GENERATED_BODY()

public:
	AGhostPawn();

	virtual void BeginPlay() override;

	// Called to notify the pawn that it is at the center of the current tile.
	virtual void OnTileCenter(const class AWalkableTile& tile) override;

	// Sets the mode.
	virtual void SetMode(EGhostMode mode);

	// The pawn turns into the specified direction.
	void TurnDirection(enum class EMovingDirection dir);

	// Returns the scatter tile.
	const class ATile* GetScatterTile() const;

protected:
	UFUNCTION() // Wrapper that executes OnBeginOverlapImpl.
	virtual void OnBeginOverlap(class UPrimitiveComponent* overlappedComponent, class AActor* otherActor, class UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);

	// Given a mode tag, returns the corresponding mode data.
	virtual UGhostModeData* TranslateModeTagToMode(EGhostMode modeTag) const;

	class AGhostAiController* AiController;

	using OnBeginOverlapSignature = void(class AActor* otherActor, class UPrimitiveComponent* otherComponent);
	TFunction<OnBeginOverlapSignature> OnBeginOverlapImpl; // How to react to an overlap with another object

	UPROPERTY(EditAnywhere, Category = "Initialization parameters")
	class ATile* ScatterTile;

	UPROPERTY(EditAnywhere)
	UGhostModeData* StandardMode;

	UPROPERTY(EditAnywhere)
	UGhostModeData* ScatterMode;
	
	UPROPERTY(EditAnywhere)
	UGhostModeData* HomeMode;
	
	UPROPERTY(EditAnywhere)
	UGhostModeData* FrightenedMode;

	UPROPERTY(EditAnywhere)
	UGhostModeData* DeadMode;
};
