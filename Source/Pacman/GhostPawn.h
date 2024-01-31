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

	// Returns the home tile.
	const class ATile* GetHomeTile() const;

	// Returns the current mode of the ghost
	const EGhostMode GetMode() const;

	// Returns the unique identifier.
	const FString GetId() const;

protected:
	UFUNCTION() // Wrapper that executes OnBeginOverlapImpl.
	virtual void OnBeginOverlap(class UPrimitiveComponent* overlappedComponent, class AActor* otherActor, class UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);

	// Given a mode tag, returns the corresponding mode data.
	virtual UGhostModeData* TranslateModeTagToMode(EGhostMode modeTag) const;

	class AGhostAiController* AiController;

	using OnBeginOverlapSignature = void(class AActor* otherActor, class UPrimitiveComponent* otherComponent);
	TFunction<OnBeginOverlapSignature> OnBeginOverlapImpl; // How to react to an overlap with another object

	UPROPERTY(EditInstanceOnly, Category = "Pacman|Maze related")
	class ATile* ScatterTile;

	UPROPERTY(EditInstanceOnly, Category = "Pacman|Maze related")
	class ATile* HomeTile;

	UPROPERTY(EditDefaultsOnly, Category = "Pacman|Ghost modes")
	UGhostModeData* StandardMode;

	UPROPERTY(EditDefaultsOnly, Category = "Pacman|Ghost modes")
	UGhostModeData* ScatterMode;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pacman|Ghost modes")
	UGhostModeData* HomeMode;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pacman|Ghost modes")
	UGhostModeData* FrightenedMode;

	UPROPERTY(EditDefaultsOnly, Category = "Pacman|Ghost modes")
	UGhostModeData* DeadMode;

	UPROPERTY(VisibleAnywhere, Category = "Pacman|Real-time info")
	EGhostMode CurrentMode;

	UPROPERTY(EditInstanceOnly, Category = "Pacman") // A unique identifier to reference this ghost from the world settings
	FString UniqueId;
};
