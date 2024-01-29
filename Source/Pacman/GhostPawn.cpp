#include "GhostPawn.h"
#include "GhostAiController.h"
#include "BoardPawnMovementComponent.h"
#include "GhostsTargetAcquisitions.h"
#include "WalkableTile.h"


AGhostPawn::AGhostPawn() {
}

void AGhostPawn::BeginPlay() {
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AGhostPawn::OnBeginOverlap); // Register how to react to overlap

	// Spawn a new AI controller and make it possess this pawn
	AiController = Cast<AGhostAiController>(GetWorld()->SpawnActor(AGhostAiController::StaticClass()));
	AiController->Possess(this);
	SetMode(EGhostMode::STANDARD); // Ghosts start in home mode TODO

	Super::BeginPlay();
}


void AGhostPawn::OnTileCenter(const AWalkableTile& tile) {
	UE_LOG(LogTemp, Display, TEXT("Ghost %s on tile center %s"), *GetName(), *tile.GetName());
	MovementComponent->SetSpeed(tile.GetType() == ETileType::TUNNEL ? TunnelSpeed : StandardSpeed); // Set the speed of the ghost based on the tile he is on
	AiController->GhostOnTileCenter(tile);
	MovementComponent->OnTileCenter(tile);
}


const ATile* AGhostPawn::GetScatterTile() const {
	return ScatterTile;
}


void AGhostPawn::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) {
	OnBeginOverlapImpl(otherActor, otherComponent); // The implementation is decided by the mode
}


void AGhostPawn::SetMode(EGhostMode mode) {
	UGhostModeData* modeData;
	switch (mode) {
	case EGhostMode::HOME:
		modeData = HomeMode;
		break;
	case EGhostMode::SCATTER:
		modeData = ScatterMode;
		break;
	case EGhostMode::STANDARD:
		modeData = StandardMode;
		break;
	case EGhostMode::FRIGHTENED:
		modeData = FrightenedMode;
		break;
	case EGhostMode::DEAD:
		modeData = DeadMode;
		break;
	case EGhostMode::ANGRY:
		modeData = AngryMode;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("AGhostPawn::SetMode unknown mode"));
		return;
	}

	AiController->SetMode(*modeData);
	StandardSpeed = modeData->StandardSpeed;
	TunnelSpeed = modeData->TunnelSpeed;
	OnBeginOverlapImpl = [&modeData](AActor* otherActor, UPrimitiveComponent* otherComponent) 
		{ return modeData->OnCollideReactionClass.GetDefaultObject()->React(otherActor, otherComponent); };
	 
	// Set the mesh (if it is nullptr use the default mesh)
	if (modeData->Mesh == nullptr) Mesh->SetStaticMesh(DefaultMesh);
	else Mesh->SetStaticMesh(modeData->Mesh);
}


void AGhostPawn::TurnDirection(EMovingDirection dir) {
	MovementComponent->SetMovingDirection(dir);
}
