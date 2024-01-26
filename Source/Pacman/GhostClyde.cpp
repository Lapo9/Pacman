#include "GhostClyde.h"
#include "GhostAiController.h"
#include "GhostsTargetAcquisitions.h"

AGhostClyde::AGhostClyde() {
	Tag = ECharacterTag::CLYDE;
}


void AGhostClyde::BeginPlay() {
	// Spawn a new AI controller and make it possess this pawn
	AiController = Cast<AGhostAiController>(GetWorld()->SpawnActor(AGhostAiController::StaticClass()));
	AiController->Init(GhostsTargetAcquisitions::Clyde); // Assign how the ghost acquires the new target tile
	AiController->Possess(this);


	Super::BeginPlay(); // This must be called afterwards because BoardPawn::BeginPlay triggers an OnTileCenter call, which needs the AiController to be present
}

