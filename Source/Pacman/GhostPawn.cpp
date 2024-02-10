#include "GhostPawn.h"
#include "GhostAiController.h"
#include "LevelSettings.h"
#include "BoardPawnMovementComponent.h"
#include "GhostsTargetAcquisitions.h"
#include "WalkableTile.h"
#include "Components/SphereComponent.h"
#include "UObject/Class.h"


AGhostPawn::AGhostPawn() : OnBeginOverlapImpl{ [](AActor* otherActor, UPrimitiveComponent* otherComponent) {} } {
	ModeIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ModeIndicator"));
	ModeIndicator->SetupAttachment(Mesh, TEXT("ModeIndicatorSocket"));
}


void AGhostPawn::BeginPlay() {
	Super::BeginPlay();
	FullCollider->OnComponentBeginOverlap.AddDynamic(this, &AGhostPawn::OnBeginOverlap); // Register how to react to overlap

	// Spawn a new AI controller and make it possess this pawn
	AiController = Cast<AGhostAiController>(GetWorld()->SpawnActor(AGhostAiController::StaticClass()));
	AiController->SetMode(*HomeModeSettings); // Set home as first default mode (it will be immediately changed by the TimeModeManager component)
	AiController->Possess(this);
}


void AGhostPawn::Init() {
	Super::Init();
	SetMode(EGhostMode::HOME); // Set home as first default mode (it will be immediately changed by the TimeModeManager component)
}


void AGhostPawn::Start() {
	Super::Start();
	SetMode(EGhostMode::HOME); // Set home as first default mode (it will be immediately changed by the TimeModeManager component)
	OnNewTile(CurrentTile);
}


void AGhostPawn::OnTileCenter(const AWalkableTile& tile) {
	AiController->GhostOnTileCenter(tile);
	Super::OnTileCenter(tile);
}


void AGhostPawn::OnNewTile(const AWalkableTile* tile) {
	Super::OnNewTile(tile);

	// If the ghost was in DEAD mode, and reached its respawn tile, go in HOME mode and teleport to the home tile
	if (CurrentMode == EGhostMode::DEAD && tile == RespawnTile) {
		SetLocation2d(HomeTile->GetCenter());
		SetMode(EGhostMode::HOME);
	}
}


void AGhostPawn::SetSettings(FGhostScheduleItem& settings) {
	BaseSpeed = settings.BaseSpeed;
	settings.Ghost = this;
}


const ATile* AGhostPawn::GetScatterTile() const {
	return ScatterTile;
}


const AWalkableTile* AGhostPawn::GetRespawnTile() const {
	return RespawnTile;
}


const AWalkableTile* AGhostPawn::GetHomeTile() const {
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
	UE_LOG(LogTemp, Display, TEXT("Setting ghost %s to mode %s (from mode %s)"), *GetName(), *UEnum::GetValueAsString<EGhostMode>(mode), *UEnum::GetValueAsString<EGhostMode>(CurrentMode));
	UGhostModeData* modeData = TranslateModeTagToMode(mode);
	if (modeData == nullptr) return;

	// If the ghost was in HOME mode, and now the mode is anything bu HOME, teleport to the respawn tile (in order to leave the home)
	if(CurrentMode == EGhostMode::HOME && mode != EGhostMode::HOME) SetLocation2d(RespawnTile->GetCenter());

	// Set the new mode parameters
	CurrentMode = mode;
	AiController->SetMode(*modeData);
	ModeSpeedMultiplier = modeData->SpeedMultiplier;
	OnBeginOverlapImpl = [modeData, this](AActor* otherActor, UPrimitiveComponent* otherComponent) 
		{ return modeData->OnCollideReactionClass.GetDefaultObject()->React(this, otherActor, otherComponent); };
	 
	// Set the mesh (if it is nullptr use the default mesh)
	if (modeData->Mesh == nullptr) {
		Mesh->SetSkeletalMesh(DefaultMesh);
		for (int i = 0; i < DefaultMaterials.Num(); ++i) Mesh->SetMaterial(i, DefaultMaterials[i]);
	}
	else {
		Mesh->SetSkeletalMesh(modeData->Mesh);
		for (int i = 0; i < modeData->Materials.Num(); ++i) Mesh->SetMaterial(i, modeData->Materials[i]);
	}

	// Show additional meshes
	ModeIndicator->SetStaticMesh(modeData->ModeIndicatorMesh);
}


void AGhostPawn::TurnDirection(EMovingDirection dir) {
	MovementComponent->SetMovingDirection(dir);
}
