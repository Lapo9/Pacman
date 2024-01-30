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


UGhostModeData* AGhostPawn::TranslateModeTagToMode(EGhostMode modeTag) const {
	switch (modeTag) {
	case EGhostMode::HOME:
		return HomeMode;
		break;
	case EGhostMode::SCATTER:
		return ScatterMode;
		break;
	case EGhostMode::STANDARD:
		return StandardMode;
		break;
	case EGhostMode::FRIGHTENED:
		return FrightenedMode;
		break;
	case EGhostMode::DEAD:
		return DeadMode;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("AGhostPawn::SetMode unknown mode"));
		return nullptr;
	}
}


void AGhostPawn::SetMode(EGhostMode mode) {
	UGhostModeData* modeData = TranslateModeTagToMode(mode);
	if (modeData == nullptr) return;

	AiController->SetMode(*modeData);
	StandardSpeed = modeData->StandardSpeed;
	TunnelSpeed = modeData->TunnelSpeed;
	OnBeginOverlapImpl = [modeData, this](AActor* otherActor, UPrimitiveComponent* otherComponent) 
		{ return modeData->OnCollideReactionClass.GetDefaultObject()->React(this, otherActor, otherComponent); };
	 
	// Set the mesh (if it is nullptr use the default mesh)
	if (modeData->Mesh == nullptr) Mesh->SetStaticMesh(DefaultMesh);
	else Mesh->SetStaticMesh(modeData->Mesh);
}


void AGhostPawn::TurnDirection(EMovingDirection dir) {
	MovementComponent->SetMovingDirection(dir);
}
