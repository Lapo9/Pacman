#pragma once

#include "CoreMinimal.h"
#include "BoardPawn.h"
#include "GhostModeData.h"
#include "GhostPawn.generated.h"


// Forward declarations
class ATile;
class AWalkableTile;
class AGhostAiController;
class UPrimitiveComponent;
enum class EMovingDirection;


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

	virtual void Init() override;

	virtual void Start() override;

	// Called to notify the pawn that it is at the center of the current tile.
	virtual void OnTileCenter(const AWalkableTile& tile) override;

	// Called to notify the pawn that it entered a new tile.
	virtual void OnNewTile(const AWalkableTile* tile) override;

	// Sets the mode.
	virtual void SetMode(EGhostMode mode);

	// The pawn turns into the specified direction.
	void TurnDirection(enum EMovingDirection dir);

	// Returns the scatter tile.
	const ATile* GetScatterTile() const;

	// Returns the respawn tile (if a ghost overlaps this tile while DEAD, it will be teleported to the home tile)
	const AWalkableTile* GetRespawnTile() const;	
	
	// Returns the home tile.
	const AWalkableTile* GetHomeTile() const;

	// Returns the current mode of the ghost
	const EGhostMode GetMode() const;

	// Returns the unique identifier.
	const FString GetId() const;

protected:
	UFUNCTION() // Wrapper that executes OnBeginOverlapImpl.
	virtual void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);

	// Given a mode tag, returns the corresponding mode data.
	virtual UGhostModeData* TranslateModeTagToMode(EGhostMode modeTag) const;

	AGhostAiController* AiController;

	using OnBeginOverlapSignature = void(AActor* otherActor, UPrimitiveComponent* otherComponent);
	TFunction<OnBeginOverlapSignature> OnBeginOverlapImpl; // How to react to an overlap with another object

	UPROPERTY(EditInstanceOnly, Category = "Pacman|Maze related") // The tile that the ghost targets when he is in scatter mode.
	ATile* ScatterTile;	
	
	UPROPERTY(EditInstanceOnly, Category = "Pacman|Maze related") // Where the pawn respawns to get out of the ghost home.
	AWalkableTile* RespawnTile;

	UPROPERTY(EditInstanceOnly, Category = "Pacman|Maze related") // It should be any tile inside the ghost home.
	AWalkableTile* HomeTile;

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
