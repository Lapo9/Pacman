#include "PacmanGameMode.h"
#include "PacmanLevelState.h"
#include "PacmanPlayerController.h"

APacmanGameMode::APacmanGameMode() {
	GameStateClass = APacmanLevelState::StaticClass();
	PlayerControllerClass = APacmanPlayerController::StaticClass();
}

void APacmanGameMode::StartPlay() {
	UE_LOG(LogTemp, Display, TEXT("APacmanGameMode::StartPlay"));
	Cast<APacmanLevelState>(GameState)->Init(); // Initialize the state
	Super::StartPlay(); // This will call all the BeginPlay() functions
}
