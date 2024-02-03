#include "PacmanGameMode.h"
#include "PacmanLevelState.h"
#include "PacmanSettings.h"
#include "GhostPawn.h"
#include "GhostModeData.h"
#include "PacmanPawn.h"
#include "PacmanGameInstance.h"
#include "PacmanPlayerController.h"

APacmanGameMode::APacmanGameMode() {
	GameStateClass = APacmanLevelState::StaticClass();
	PlayerControllerClass = APacmanPlayerController::StaticClass();

	TimeModeManager = CreateDefaultSubobject<UTimeModeManager>(TEXT("TimeModeManager"));
}


void APacmanGameMode::StartPlay() {
	UE_LOG(LogTemp, Display, TEXT("APacmanGameMode::StartPlay"));
	Cast<APacmanLevelState>(GameState)->Init(); // Initialize the state
	
	// Set the settings for this level, based on the level number we are into
	auto& levelsSettings = Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->LevelsSettings;
	auto levelSettings = levelsSettings[FMath::Max(levelsSettings.Num() - 1, (int)Cast<UPacmanGameInstance>(GetWorld()->GetGameInstance())->GetLevel())];
	TimeModeManager->SetSettings(levelSettings);
	
	// Activate all board pawns and set their speeds
	for (auto& pawnItem : levelSettings->GhostsSchedule) {
		pawnItem.Ghost->SetBaseSpeed(pawnItem.BaseSpeed);
	}
	const auto& boardPawns = Cast<APacmanLevelState>(GameState)->GetBoardPawns();
	for (auto& pawn : boardPawns) {
		pawn->StartMoving(); // Start gameplay
		if (pawn->IsA(APacmanPawn::StaticClass())) {
			pawn->SetBaseSpeed(levelSettings->PacmanBaseSpeed);
		}
	}
	Super::StartPlay(); // This will call all the BeginPlay() functions

	TimeModeManager->Start(); // Start play
}


// Called when Pacman eats a power pellet, turns all ghosts into FRIGHTENED mode (unless they are in DEAD or HOME mode).
void APacmanGameMode::NotifyPowerPelletEaten() const {
	TimeModeManager->NotifyPowerPelletEaten();
}


void APacmanGameMode::NotifyGhostEaten(AGhostPawn& ghost) const {
	ghost.SetMode(EGhostMode::DEAD);
	
	auto& gameState = *Cast<APacmanLevelState>(GameState);
	auto& currentPowerPellet = gameState.GetCurrentPowerPelletActivation();
	int n = currentPowerPellet.GhostsEatenInThisPowerPellet;

	gameState.AddPoints(200 * FMath::Pow(2.f, n-1));

	// If all ghosts got eaten, end power pellet activation
	if (n >= gameState.GetBoardPawns().Num()) {
		TimeModeManager->NotifyPowerPelletEnded();
	}
}

void APacmanGameMode::NotifyAvailableFoodDecreasedBy1(unsigned int remainingFood) const {
	TimeModeManager->NotifyStandardFoodDecreasedBy1(remainingFood);
}


void APacmanGameMode::NotifyPacmanDead() {
	// Stop all pawns
	const auto& boardPawns = Cast<APacmanLevelState>(GameState)->GetBoardPawns();
	for (auto& pawn : boardPawns) {
		pawn->StopMoving();
	}
	//TODO Pacman and ghosts should play an animation/sound + something should appear on screen

	auto restart = [&boardPawns]() {
		for (auto& pawn : boardPawns) {
			pawn->SetLocation2d(pawn->GetSpawnTile()->GetActorLocation()); // Place the pawn at the spawn
			pawn->StartMoving(); // Resume gameplay
		}};

	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, restart, 3, false); // Set a timer to respart gameplay in 3 seconds
}


void APacmanGameMode::NotifyGameOver() {
}


void APacmanGameMode::SetGhostModeUnless(AGhostPawn& ghost, EGhostMode mode, const TArray<EGhostMode>& dontChange) const {
	if (!dontChange.Contains(ghost.GetMode())) ghost.SetMode(mode);
}


void APacmanGameMode::SetGhostsModeUnless(EGhostMode mode, const TArray<EGhostMode>& dontChange) const {
	const auto& boardPawns = Cast<APacmanLevelState>(GameState)->GetBoardPawns();
	for (auto& pawn : boardPawns) {
		if (auto ghost = Cast<AGhostPawn>(pawn); ghost != nullptr) {
			SetGhostModeUnless(*ghost, mode, dontChange);
		}
	}
}