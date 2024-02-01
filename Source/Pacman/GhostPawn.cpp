#include "GhostPawn.h"
#include "GhostAiController.h"
#include "BoardPawnMovementComponent.h"
#include "GhostsTargetAcquisitions.h"
#include "WalkableTile.h"
#include "UObject/Class.h"


AGhostPawn::AGhostPawn() {
}


void AGhostPawn::BeginPlay() {
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AGhostPawn::OnBeginOverlap); // Register how to react to overlap

	// Spawn a new AI controller and make it possess this pawn
	AiController = Cast<AGhostAiController>(GetWorld()->SpawnActor(AGhostAiController::StaticClass()));
	AiController->Possess(this);

	// Apply the default mesh and material (they will later be changed when mode changes)
	Mesh->SetStaticMesh(DefaultMesh);
	Mesh->SetMaterial(0, DefaultMaterial);

	Super::BeginPlay();
}


void AGhostPawn::OnTileCenter(const AWalkableTile& tile) {
	Super::OnTileCenter(tile);

	AiController->GhostOnTileCenter(tile);
	MovementComponent->OnTileCenter(tile);
}


const ATile* AGhostPawn::GetScatterTile() const {
	return ScatterTile;
}


const ATile* AGhostPawn::GetHomeTile() const {
	return HomeTile;
}


const EGhostMode AGhostPawn::GetMode() const {
	return CurrentMode;
}


const FString AGhostPawn::GetId() const {
	return UniqueId;
}


void AGhostPawn::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) {
	OnBeginOverlapImpl(otherActor, otherComponent); // The implementation is decided by the mode
}


UGhostModeData* AGhostPawn::TranslateModeTagToMode(EGhostMode modeTag) const {
	switch (modeTag) {
	case EGhostMode::HOME:
		return HomeModeSettings;
		break;
	case EGhostMode::SCATTER:
		return ScatterModeSettings;
		break;
	case EGhostMode::CHASE:
		return ChaseModeSettings;
		break;
	case EGhostMode::FRIGHTENED:
		return FrightenedModeSettings;
		break;
	case EGhostMode::DEAD:
		return DeadModeSettings;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("AGhostPawn::SetMode unknown mode"));
		return nullptr;
	}
}


void AGhostPawn::SetMode(EGhostMode mode) {
	UE_LOG(LogTemp, Display, TEXT("Setting ghost %s to mode %s"), *GetName(), *UEnum::GetValueAsString<EGhostMode>(mode));
	UGhostModeData* modeData = TranslateModeTagToMode(mode);
	if (modeData == nullptr) return;

	CurrentMode = mode;
	AiController->SetMode(*modeData);
	ModeSpeedMultiplier = modeData->SpeedMultiplier;
	OnBeginOverlapImpl = [modeData, this](AActor* otherActor, UPrimitiveComponent* otherComponent) 
		{ return modeData->OnCollideReactionClass.GetDefaultObject()->React(this, otherActor, otherComponent); };
	 
	// Set the mesh (if it is nullptr use the default mesh)
	if (modeData->Mesh == nullptr) {
		Mesh->SetStaticMesh(DefaultMesh);
		Mesh->SetMaterial(0, DefaultMaterial);
	}
	else Mesh->SetStaticMesh(modeData->Mesh);
}


void AGhostPawn::TurnDirection(EMovingDirection dir) {
	MovementComponent->SetMovingDirection(dir);
}
