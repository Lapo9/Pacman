#pragma once

#include "CoreMinimal.h"
#include "BoardPawn.h"
#include "GhostModeData.h"
#include "GhostPawn.generated.h"


// A GhostPawn is a BoardPawn controlled by a basic AI algorithm.
// GhostPawn AI decision making is triggered when the BoardPawn receives the OnTileCenter event from a WalkableTile; at this point the GhostPawn will choose a new direction where to go.
// A GhostPawn can be placed in different modes by the TimeModeManager. Each mode has a different decision making algorithm.
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

	UPROPERTY(EditAnywhere, Category = "Pacman|Ghost modes")
	UGhostModeData* ChaseModeSettings;

	UPROPERTY(EditAnywhere, Category = "Pacman|Ghost modes")
	UGhostModeData* ScatterModeSettings;
	
	UPROPERTY(EditAnywhere, Category = "Pacman|Ghost modes")
	UGhostModeData* HomeModeSettings;
	
	UPROPERTY(EditAnywhere, Category = "Pacman|Ghost modes")
	UGhostModeData* FrightenedModeSettings;

	UPROPERTY(EditAnywhere, Category = "Pacman|Ghost modes")
	UGhostModeData* DeadModeSettings;

	UPROPERTY(VisibleAnywhere, Category = "Pacman|Real-time info")
	EGhostMode CurrentMode;

	UPROPERTY(EditInstanceOnly, Category = "Pacman") // A unique identifier to reference this ghost from the world settings
	FString UniqueId;

	UPROPERTY(EditAnywhere, Category = "Pacman") // An additional mesh that can be placed abve the ghost to indicate its mode.
	UStaticMeshComponent* ModeIndicator;
};
