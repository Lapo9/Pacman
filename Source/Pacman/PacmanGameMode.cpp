#include "PacmanGameMode.h"
#include "PacmanLevelState.h"
#include "GhostPawn.h"
#include "GhostModeData.h"
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


// Called when Pacman eats a power pellet, turns all ghosts into frightened mode.
void APacmanGameMode::NotifyPowerPelletEaten() const {
	SetGhostsMode(EGhostMode::FRIGHTENED);

	// Set the timer for this power pellet
	auto& timer = Cast<APacmanLevelState>(GameState)->GetCurrentPowerPelletActivation().Countdown;
	GetWorldTimerManager().SetTimer(timer, this, &APacmanGameMode::NotifyPowerPelletEnded, 10.f, false);
}


void APacmanGameMode::SetGhostsMode(EGhostMode mode) const {
	const auto& boardPawns = Cast<APacmanLevelState>(GameState)->GetBoardPawns();
	for (auto& pawn : boardPawns) {
		if (auto ghost = Cast<AGhostPawn>(pawn); ghost != nullptr) {
			ghost->SetMode(mode);
		}
	}
}


// Called when the timer of the power pellet ended, or when all ghosts got eaten.
void APacmanGameMode::NotifyPowerPelletEnded() const {
	UE_LOG(LogTemp, Display, TEXT("Power pellet ended"));
	NotifyChangeOfModeDueToTime();
}


void APacmanGameMode::NotifyChangeOfModeDueToTime() const {
	//TODO check time passed and select the appropriate mode
}


void APacmanGameMode::NotifyGhostEaten(AGhostPawn& ghost) const {
	ghost.SetMode(EGhostMode::DEAD);
	
	auto& gameState = *Cast<APacmanLevelState>(GameState);
	auto& currentPowerPellet = gameState.GetCurrentPowerPelletActivation();
	int n = currentPowerPellet.GhostsEatenInThisPowerPellet;

	gameState.AddPoints(200 * FMath::Pow(2.f, n-1));

	// If all ghosts got eaten, end power pellet activation
	if (n >= gameState.GetBoardPawns().Num()) {
		NotifyPowerPelletEnded();
	}
}
