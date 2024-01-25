#include "GhostPawn.h"
#include "GhostAiController.h"


void AGhostPawn::BeginPlay() {
	// Spawn a new AI controller and make it possess this pawn
	auto aiController = Cast<AGhostAiController>(GetWorld()->SpawnActor(AGhostAiController::StaticClass()));
	aiController->Possess(this);
}
