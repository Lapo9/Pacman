#include "GhostPawn.h"
#include "GhostAiController.h"
#include "BoardPawnMovementComponent.h"
#include "GhostsTargetAcquisitions.h"
#include "Tile.h"


AGhostPawn::AGhostPawn() {
	Tag = ECharacterTag::BLINKY;
}

void AGhostPawn::BeginPlay() {
	// Spawn a new AI controller and make it possess this pawn
	AiController = Cast<AGhostAiController>(GetWorld()->SpawnActor(AGhostAiController::StaticClass()));
	AiController->Init(GhostsTargetAcquisitions::Blinky); // Assign how the ghost acquires the new target tile
	AiController->Possess(this);

	Super::BeginPlay();
}


void AGhostPawn::OnTileCenter(const ATile& tile) {
	UE_LOG(LogTemp, Display, TEXT("Ghost %s on tile center %s"), *GetName(), *tile.GetName());
	MovementComponent->SetSpeed(tile.GetType() == ETileType::TUNNEL ? TunnelSpeed : StandardSpeed); // Set the speed of the ghost based on the tile he is on
	AiController->GhostOnTileCenter(tile);
}


void AGhostPawn::TurnDirection(EMovingDirection dir) {
	MovementComponent->SetMovingDirection(dir);
}
