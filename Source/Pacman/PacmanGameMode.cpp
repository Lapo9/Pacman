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
	UE_LOG(LogTemp, Display, TEXT("APacmanGameMode::StartPlay called"));
	Super::StartPlay(); // This will call all the BeginPlay() functions
	Init(); // Initialize the state and the mode manager (here we are sure that BeginPlay has already been executed on everything)
}


// Initializes the games state and the mode manager.
void APacmanGameMode::Init() {
	UE_LOG(LogTemp, Display, TEXT("Initializing Pacman game mode..."));
	Cast<APacmanLevelState>(GameState)->Init();
	auto& levelsSettings = Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->LevelsSettings;
	TimeModeManager->Init(levelsSettings[FMath::Min(levelsSettings.Num() - 1, (int)Cast<UPacmanGameInstance>(GetWorld()->GetGameInstance())->GetLevel())]);
	UE_LOG(LogTemp, Display, TEXT("Initialization done!"));
}


void APacmanGameMode::Start() {
	UE_LOG(LogTemp, Display, TEXT("Starting Pacman game mode..."));
	Cast<APacmanLevelState>(GameState)->Start(); // Start the actors
	TimeModeManager->Start(); // Start the schedules
	UE_LOG(LogTemp, Display, TEXT("Starting phase done!"));
}


// Called when Pacman eats a power pellet, turns all ghosts into FRIGHTENED mode (unless they are in DEAD or HOME mode).
void APacmanGameMode::NotifyPowerPelletEaten() const {
	TimeModeManager->NotifyPowerPelletEaten();
}


void APacmanGameMode::NotifyGhostEaten(AGhostPawn& ghost) const {
	ghost.SetMode(EGhostMode::DEAD);
	TimeModeManager->NotifyGhostDied(ghost);
	
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
		pawn->Stop();
	}
	//TODO Pacman and ghosts should play an animation/sound + something should appear on screen

	auto restart = [&boardPawns, this]() {
		for (auto pawn : boardPawns) {
			pawn->Start(); // Resume gameplay
			pawn->SetLocation2d(pawn->GetSpawnTile()->GetActorLocation()); // Place the pawn at the spawn
			if (auto ghost = Cast<AGhostPawn>(pawn); ghost) TimeModeManager->ActivateGhost(*ghost);
		}};

	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, restart, 3, false); // Set a timer to respart gameplay in 3 seconds
}


void APacmanGameMode::NotifyGameOver() {
	TimeModeManager->NotifyLevelEnded(); // Basically stops all timers
	auto& boardPawns = Cast<APacmanLevelState>(GameState)->GetBoardPawns();
	for (auto pawn : boardPawns) pawn->Stop();
	//TODO Show point screen
}


void APacmanGameMode::NotifyLevelCleared() {
	auto& boardPawns = Cast<APacmanLevelState>(GameState)->GetBoardPawns();
	for (auto& pawn : boardPawns) {
		pawn->Stop(); // Resume gameplay
	}

	TimeModeManager->NotifyLevelEnded(); // Basically stops all timers
	Cast<UPacmanGameInstance>(GetWorld()->GetGameInstance())->LevelEnded(true); // Notify the game instance

	// Initialize mew level TODO
	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, [this]() {Init(); Start();}, 2.f, false);
}


void APacmanGameMode::LoadLevelSettings() {
	// Set the settings for this level, based on the level number we are into
	auto& levelsSettings = Cast<APacmanSettings>(GetWorld()->GetWorldSettings())->LevelsSettings;
	auto levelSettings = levelsSettings[FMath::Max(levelsSettings.Num() - 1, (int)Cast<UPacmanGameInstance>(GetWorld()->GetGameInstance())->GetLevel())];
	TimeModeManager->Init(levelSettings); // Set all the settings in the specified classes (e.g. sets the speed variable in a ghost)
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